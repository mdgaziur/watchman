#! /bin/env python
W_VER = "0.1"

import os, sys
import platform

if platform.system().lower() != "linux":
    print("Watchman can only run in linux")
    exit(1)

if os.getuid() != 0:
    print("You need to run the installer as root")
    exit(1)
print("Checking for existing installation...")
if os.path.exists("/usr/bin/watchman"):
    print("Another installation already exists. Do you want to continue? [y/N]")
    i = input()
    if i not in "Yy" or i == "":
        print("Bye.")
        exit(1)
print("Compiling...")

import subprocess

p = subprocess.Popen("./build.py")
p.wait()
if p.returncode != 0:
    print("Build failed. Output: ")
    print(p.stderr.read().decode())
else:
    print("Copying watchman to /usr/bin...")
    import shutil

    shutil.copy("./build/watchman", "/usr/bin/watchman")
    print("Installation complete!")
    print("Removing build files...")
    shutil.rmtree("build")
    print("Done")