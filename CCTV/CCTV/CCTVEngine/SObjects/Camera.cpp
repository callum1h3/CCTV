#include "Camera.h"
#include "../../Engine/Renderer.h"
#include "../../Engine/Input.h"
#include "../../Engine/OSHandler.h"
#include "../../Engine/Time.h"
#include "../../Application.h"
#include <sstream>
#include <fstream>
#include "jpeg-compressor/jpgd.h"

static bool CheckPortStatus(int PortNo, const char* IPAddress)
{
	SOCKET s;

	//Start up Winsock…
	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	//Fill out the information needed to initialize a socket…
	SOCKADDR_IN target;               //Socket address information

	target.sin_family = AF_INET;      // address family Internet
	target.sin_port = htons(PortNo); //Port to connect on
	target.sin_addr.s_addr = inet_addr(IPAddress); //Target IP

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (s == INVALID_SOCKET)
	{
		return false; //Couldn't create the socket
	}

	if (connect(s, (SOCKADDR*)&target, sizeof(target)) == SOCKET_ERROR)
	{
		return false;                 //Couldn't connect
	}
	else
	{
		return true;                  //Success
	}
}

#define CAMERA_ICON_SIZE 64
#define CAMERA_ICON_SIZE_HALF CAMERA_ICON_SIZE / 2

static Engine::Texture2D* camera_icon = nullptr;


const char* CameraTool::GetIcon()
{
	return "camera.png";
}

bool CameraTool::OnPlace(glm::vec2 cursor_position)
{
	SCamera* camera = new SCamera();
	camera->ip = "";
	camera->position = cursor_position - glm::vec2(CAMERA_ICON_SIZE_HALF, CAMERA_ICON_SIZE_HALF);



	CCTVManager::AddSObject(camera);

	return true;
}

void CameraTool::OnDisplay(glm::vec2 cursor_position)
{
	if (camera_icon == nullptr)
		camera_icon = Engine::Resources::Load<Engine::Texture2D>("camera.png");

	Engine::Renderer::DrawRect(cursor_position - glm::vec2(CAMERA_ICON_SIZE_HALF, CAMERA_ICON_SIZE_HALF), glm::vec2(CAMERA_ICON_SIZE, CAMERA_ICON_SIZE), glm::vec4(1, 1, 1, 1), camera_icon);


}

SOCKET socket_create_http(const char* hostname, unsigned short port)
{
	hostent* host = gethostbyname(hostname);
	if (host)
	{
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock != INVALID_SOCKET)
		{
			SOCKADDR_IN SockAddr = {};
			SockAddr.sin_family = AF_INET;
			SockAddr.sin_port = htons(port);
			SockAddr.sin_addr.s_addr = *reinterpret_cast<unsigned long*>(host->h_addr);

			if (connect(sock, reinterpret_cast<SOCKADDR*>(&SockAddr), sizeof(SockAddr)) != SOCKET_ERROR)
				return sock;

			closesocket(sock);
		}
	}

	return INVALID_SOCKET;
}

bool sendAll(SOCKET sock, const void* data, size_t data_size)
{
	const char* pdata = static_cast<const char*>(data);

	while (data_size > 0)
	{
		int sent = send(sock, pdata, data_size, 0);
		if (sent == SOCKET_ERROR)
			return false;

		pdata += sent;
		data_size -= sent;
	}

	return true;
}

SOCKET SCamera::ConnectToImageStream(std::string target_ip)
{
	SOCKET s = socket_create_http(target_ip.c_str(), 8000);
	if (s != INVALID_SOCKET)
	{
		std::ostringstream request;
		request << "GET /stream.mjpg HTTP/1.1\r\n"; // or HTTP/1.0
		request << "Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\n";
		request << "Accept-Encoding: gzip, deflate\r\n";
		request << "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n";
		request << "Connection: keep-alive\r\n";
		request << "Host: ";
		request << target_ip.c_str();
	    request << ":8000\r\n";
		//request << "Referer: http://192.168.1.84:8000/index.html\r\n";
		request << "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36\r\n";
		request << "\r\n";

		std::string request_string = request.str();
		sendAll(s, request_string.c_str(), request_string.size());
	}

	return s;
}

bool SCamera::HasImage()
{
	return false;
}

bool SCamera::ShouldBeGenerating(std::string target_ip)
{
	if (!validCamera)
		return false;

	if (!isRetrievingCamera)
		return false;

	if (ip != target_ip)
		return false;

	if ((lastRequestTime + 2) < Engine::Time::FixedTime())
		return false;

	return true;
}

void SCamera::GatherImageThread(SCamera* target, std::string target_ip)
{
	SOCKET socket;
		
	while (target->ShouldBeGenerating(target_ip))
	{
		socket = target->ConnectToImageStream(target_ip);
		if (socket != INVALID_SOCKET)
			break;
		std::cout << "Failed to connect to camera socket!\n";
	}

	char buffer[8192] = { 0 };
	bool frameStart = false;
	std::vector<char> frame_buffer;
	bool previousFF = false;

	char ff = 0xFF;
	char d8 = 0xD8;
	char d9 = 0xD9;

	float tickSpeed = 24;
	float sleepTime = (1.0f / tickSpeed) / 1000.0f;

	std::cout << "Successfully connected to socket!\n";
	while (target->ShouldBeGenerating(target_ip) && socket != INVALID_SOCKET)
	{
		int valread = recv(socket, buffer, 8192 - 1, 0);

		if (valread > 0)
		{
			bool finishedFrameInPacket = false;
			for (int i = 0; i < valread; i++)
			{
				char character = buffer[i];

				if (previousFF)
				{
					if (character == d8)
					{
						// Start
						frameStart = true;
						frame_buffer.clear();
					}

					if (character == d9)
					{
						// Frame End
						frameStart = false;

						if (frame_buffer.size() > 0)
						{
							//std::cout << "Compiled Frame!\n";

							finishedFrameInPacket = true;
							frame_buffer[0] = ff;
							frame_buffer[1] = d8;

							frame_buffer.push_back(character);

							int width, height, comps;
							unsigned char* data = jpgd::decompress_jpeg_image_from_memory((unsigned char*)&frame_buffer[0], frame_buffer.size(), &width, &height, &comps, 3);

							if (target->queuedData != nullptr)
							{
								delete data;
							}
							else if (data != nullptr)
							{
								ImageData* container = new ImageData();
								container->width = width;
								container->height = height;
								container->data = data;
								container->camera = target;

								target->queuedData = container;
							}

							//std::ofstream myfile;
							//myfile.open(Engine::OSHandler::GetPath() + "/aaaa.jpeg", std::ios::binary);

							//copy(frame_buffer.cbegin(), frame_buffer.cend(),
							//	std::ostreambuf_iterator<char>(myfile));

							//myfile.close();
						}
					}

					previousFF = false;
				}
				else
					previousFF = (character == ff);

				if (frameStart)
				{
					frame_buffer.push_back(character);
				}
			}
		}

		Sleep(0.5f);
	}

	std::cout << "Cleaning up socket thread!\n";
	closesocket(socket);
	target->isRetrievingCamera = false;
}

void SCamera::GenerateImage()
{
	if (isRetrievingCamera)
		return;
	
	if (lastCheckedIP != ip)
		return;

	if (!validCamera)
		return;
	isRetrievingCamera = true;

	camera_thread = std::thread(GatherImageThread, this, ip);
	camera_thread.detach();
}

Engine::Texture2D* SCamera::GetTexture()
{
	lastRequestTime = Engine::Time::FixedTime();
	if (!isRetrievingCamera)
		GenerateImage();

	return camera_texture;
}

void SCamera::Validate()
{
	if (sshCooldown > Engine::Time::FixedTime())
		return;

	sshCooldown = Engine::Time::FixedTime() + 0.25f;

	// Checking if the device is online (SSH is valid)
	validIP = CheckPortStatus(22, ip.c_str());

	if (validIP)
	{
		// Checking if camera software is already up.
		bool _validCamera = CheckPortStatus(8000, ip.c_str());

		if (_validCamera)
			errorWithCamera = false;

		if (_validCamera && _validCamera == validCamera)
			return;
		validCamera = _validCamera;

		if (!validCamera && !errorWithCamera)
		{
			std::cout << "Installing dependances onto device!\n";
			std::string command = std::string("python " + Engine::OSHandler::GetPath() + "/python/setup.py " + ip + " raspberry pi");
			system(command.c_str());

			//std::string start_upcommand = std::string("python " + Engine::OSHandler::GetPath() + "/python/startup.py " + ip + " raspberry pi");
			//system(start_upcommand.c_str());


			std::cout << "Checking if device is online!\n";
			// Checks if the camera is up three times then gives up
			for (int i = 0; i < 8; i++)
			{
				validCamera = CheckPortStatus(8000, ip.c_str());
				if (validCamera)
					break;
			}
		}
		
		// If the camera is still not booted up then theres a unrecoverable error the operator needs to fix manually.
		if (!validCamera)
		{
			std::cout << "Camera failed to start up!\n";
			errorWithCamera = true;
		}
		else
		{
			std::cout << "Camera successfully started up!\n";
			lastCheckedIP = ip;
		}

		return;
	}

	validCamera = false;
}

void SCamera::OnSettings(glm::ivec2 screenSize)
{
	Engine::Font font = Engine::Renderer::LoadFont(Engine::OSHandler::GetPath() + "/fonts/FiraCode-SemiBold.ttf");

	glm::vec2 middle = screenSize / 2;

	float ip_title_size = font.GetTextWidth(.1, "Device IP Address:");
	Engine::Renderer::DrawTextA(middle + glm::vec2(-(ip_title_size / 2), 60), glm::vec4(1, 1, 1, 1), .1, font, "Device IP Address:");


	Engine::Renderer::DrawTextInput(middle, .06, &ip, font, 64, "Please enter device IP");
}

bool SCamera::IsUseable()
{
	if (errorWithCamera)
		return false;
	return validCamera && validIP;
}

void SCamera::UpdateWindow()
{
	if (window == nullptr)
		return;

	if (window->ShouldClose())
	{
		window->Close();
		delete window;
		window = nullptr;
		return;
	}

	Engine::Texture2D* _camera_tex = GetTexture();

	window->MakeCurrentContext();
	window->PollEvents();

	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, window->GetSize().x, window->GetSize().y);


	if (window->texture != _camera_tex)
	{
		if (window->texture != nullptr)
		{
			window->DeleteTexture();
			window->MakeCurrentContext();
			window->texture = nullptr;
		}

		unsigned char* data = _camera_tex->GetData();

		if (data == nullptr)
			std::cout << "Window Recieved NULL Data!\n";
	
		glGenTextures(1, &window->self_texture);
		glBindTexture(GL_TEXTURE_2D, window->self_texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _camera_tex->GetWidth(), _camera_tex->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, window->self_texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		window->texture = _camera_tex;
	}

	if (window->self_texture != -1)
	{
		glm::vec2 image_size = glm::vec2(_camera_tex->GetWidth(), _camera_tex->GetHeight());
		
		window->shader->Use();
		glUniform1i(window->shader->UniformGetLocation("cameraTexture"), 0);
		window->shader->UniformSetVec2(10, window->GetSize());
		window->shader->UniformSetVec2(11, image_size);
		glBindVertexArray(window->vao);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, window->self_texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glfwSwapBuffers(window->Get());
	
	Application::GetWindow()->MakeCurrentContext();
}

void SCamera::OnDisplay()
{
	// Update Window
	UpdateWindow();

	if (camera_icon == nullptr)
		camera_icon = Engine::Resources::Load<Engine::Texture2D>("camera.png");

	glm::vec2 size = glm::vec2(CAMERA_ICON_SIZE, CAMERA_ICON_SIZE);
	if (Engine::Renderer::IsMouseWithinBounds(position, position + size))
	{
		if (!CCTVManager::isEditting)
		{
			Engine::Texture2D* _camera_tex = GetTexture();

			if (_camera_tex != nullptr)
			{
				float cwidth = (float)_camera_tex->GetWidth();
				float cheight = (float)_camera_tex->GetHeight();
				float cwidthratio = cwidth / cheight;
				float cheightratio = cheight / cwidth;
				glm::vec2 csize = glm::vec2(cwidthratio * 400, cheightratio * 400);
				glm::vec2 rect_position = position + glm::vec2(size.x / 2, size.y * 1.5f) + glm::vec2(csize.x * -0.5f, 0);

				Engine::Renderer::DrawRect(rect_position - glm::vec2(2, 2), csize + glm::vec2(4,4), glm::vec4(1, 1, 1, 1));
				Engine::Renderer::DrawRect(rect_position, csize, glm::vec4(1, 1, 1, 1), _camera_tex);
			}
		}

		Engine::Renderer::DrawRect(position, size, glm::vec4(1, 1, 1, .2));

		if (Engine::Input::IsMouseDown(GLFW_MOUSE_BUTTON_LEFT) && !CCTVManager::IsSettingsOpen())
		{
			if (CCTVManager::isEditting)
			{
				CCTVManager::selected_object = this;
				CCTVManager::SettingsToggle();
			}
			else
				OpenWindow();
		}
		
	}

	glm::vec4 col = glm::vec4(1, 0, 0, 1);
	if (validIP)
		col = glm::vec4(1, .5f, 0, 1);
	if (validCamera)
		col = glm::vec4(1, 1, 1, 1);

	Engine::Renderer::DrawRect(position, size, col, camera_icon);
}

void SCamera::OnDispose()
{
	if (camera_texture != nullptr)
		camera_texture->Dispose();
	
	if (window != nullptr)
	{
		window->Close();
		delete window;
		window = nullptr;
	}
}

// Queues image to be motion detected.
void SCamera::UpdateMotionDetection()
{
	double t = Engine::Time::FixedTime();

	if (greyscaleCooldown > t)
		return;

	GetTexture();

	//greyscaleCooldown = t + 5;
	//camera_texutre_greyscaled

	//camera_texutre_greyscaled

	//std::vector<char> new_greyscaled;
}

#include <iomanip>
#include <ctime>
#include "jpeg-compressor/jpge.h"

void SCamera::OnMDRecieveTexture()
{
	double t = Engine::Time::FixedTime();
	if (greyscaleCooldown > t)
		return;

	// Puts a cooldown on motion detection

	Engine::Texture2D* texture = GetTexture();
	if (texture == nullptr)
		return;

	int pixel_count = texture->GetWidth() * texture->GetHeight();
	unsigned char* texture_data = texture->GetData();
	std::vector<char> new_greyscaled;

	bool existingFrame = (camera_texutre_greyscaled.size() > 0) && (texture->GetWidth() == lastGSWidth) && (texture->GetHeight() == lastGSHeight);

	double averageDifference = 0;
	for (int i = 0; i < pixel_count; i++)
	{
		int index = i * 3;

		char r = texture_data[i];
		char g = texture_data[i + 1];
		char b = texture_data[i + 2];

		char grey = (0.11 * r) + (0.59 * g) + (0.3 * b);
		if (existingFrame)
		{
			double difference = glm::abs((double)camera_texutre_greyscaled[i] - (double)grey);
			averageDifference += difference;
		}
		new_greyscaled.push_back(grey);

	}

	if (existingFrame)
	{
		float a = averageDifference / new_greyscaled.size();
		if (a > 10)
		{
			greyscaleCooldown = t + 1.0f;
			lastMovementDetected = t;

			auto _t = std::time(nullptr);
			auto tm = *std::localtime(&_t);

			std::ostringstream oss;
			oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
			auto str = oss.str();

			std::string camera_path = Engine::OSHandler::GetPath() + "/camera/" + ip + "/";
			std::filesystem::create_directories(camera_path);

			jpge::compress_image_to_jpeg_file((camera_path + str + ".jpeg").c_str(), texture->GetWidth(), texture->GetHeight(), 3, texture_data);
		}
		else
			greyscaleCooldown = t + 8;
	}
	else
		greyscaleCooldown = t + 8;

	camera_texutre_greyscaled.clear();
	camera_texutre_greyscaled = new_greyscaled;
	lastGSWidth = texture->GetWidth();
	lastGSHeight = texture->GetHeight();
	


}

void SCamera::OnSave()
{

}

int SCamera::GetID()
{
	return 0;
}

// Making it so camera object displays ontop of everything.
int SCamera::GetOrder()
{
	return 1;
}

bool SCamera::IsLastCheckedIPCurrent()
{
	return ip == lastCheckedIP;
}

const float screenQuadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

const float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	0,  1.0f,  0.0f, 1.0f,
	0, 0,  0.0f, 0.0f,
	 1.0f, 0,  1.0f, 0.0f,

	0,  1.0f,  0.0f, 1.0f,
	 1.0f, 0,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};



void SCamera::OpenWindow()
{
	if (window != nullptr)
		return;

	if (!validCamera)
		return;

	if (!IsLastCheckedIPCurrent())
		return;

	window = new CameraWindow();
	window->Init();
	window->SetWindowName(ip.c_str());
	window->MakeCurrentContext();

	// Creating Screen Quad Buffer
	glGenVertexArrays(1, &window->vao);
	glGenBuffers(1, &window->vbo);
	glBindVertexArray(window->vao);
	glBindBuffer(GL_ARRAY_BUFFER, window->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	Engine::Shader* shader = Engine::ShaderManager::CopyShader("cameraview");
	window->shader = shader;

	Application::GetWindow()->MakeCurrentContext();
}

void CameraWindow::DeleteTexture()
{
	MakeCurrentContext();
	if (self_texture != -1)
		glDeleteTextures(1, &self_texture);
	self_texture = -1;
	Application::GetWindow()->MakeCurrentContext();
}

void CameraWindow::Close()
{
	DeleteTexture();
	Dispose();
}

ADD_TOOL(new CameraTool());
ADD_SOBJECT(new SCamera());