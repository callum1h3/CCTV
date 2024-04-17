import paramiko
import sys

# Update the next three lines with your
# server's information

host = sys.argv[1]
username = sys.argv[2]
password = sys.argv[3]

commands = [
    "screen -d -m python3 bookish-doodle/camera.py"
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
