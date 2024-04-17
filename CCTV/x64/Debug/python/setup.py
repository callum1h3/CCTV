import paramiko
import sys

# Update the next three lines with your
# server's information

host = sys.argv[1]
username = sys.argv[2]
password = sys.argv[3]

commandsold = [
   "sudo apt-get --assume-yes install python3",
    "sudo apt-get --assume-yes install git-all",
    "sudo apt-get --assume-yes install screen",
    "pip3 install io",
    "pip3 install wheel",
    "pip3 install picamera",
    "pip3 install logging",
    "pip3 install socketserver",
    "pip3 install threading",
    "pip3 install http",
    "git clone https://github.com/callum1h3/bookish-doodle",
    "screen -d -m python3 bookish-doodle/camera.py &"
]


commands = [
    "screen -d -m python3 bookish-doodle/camera.py &"
]

client = paramiko.client.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(host, username=username, password=password)

for x in commands:
    _stdin, _stdout,_stderr = client.exec_command(x)
    print(_stdout.read().decode())

client.close()

print("Finished!")
exit(1)
