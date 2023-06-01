#!/usr/bin/env python3
import subprocess
import sys
import os
import requests
from tqdm import tqdm
import zipfile
import shutil


def getEmbedded(placement: str):
    major, minor, micro, _, _ = sys.version_info[:]
    print(f"Downloading embedded python version: {major}.{minor}.{micro}")
    url = f"https://www.python.org/ftp/python/{major}.{minor}.{micro}/python-{major}.{minor}.{micro}-embed-amd64.zip"
    response = requests.get(url, stream=True)
    total_size_in_bytes = int(response.headers.get("content-length", 0))
    block_size = 1024
    progress_bar = tqdm(total=total_size_in_bytes, unit="iB", unit_scale=True)
    zip_file = f"python-{major}.{minor}.{micro}-embed-amd64.zip"
    with open(zip_file, "wb") as embedded_zip:
        for chunk in response.iter_content(chunk_size=block_size):
            progress_bar.update(len(chunk))
            if chunk:
                embedded_zip.write(chunk)
    with zipfile.ZipFile(zip_file, "r") as zip_ref:
        zip_ref.extractall(placement)
    os.remove(zip_file)


def getPip(embedded_path: str):
    major, minor, _, _, _ = sys.version_info[:]
    cwd = os.getcwd()
    os.chdir(embedded_path)
    url = "https://bootstrap.pypa.io/get-pip.py"
    response = requests.get(url, stream=True)
    total_size_in_bytes = int(response.headers.get("content-length", 0))
    block_size = 128
    progress_bar = tqdm(total=total_size_in_bytes, unit="iB", unit_scale=True)
    with open("get-pip.py", "wb") as pip_py:
        for chunk in response.iter_content(chunk_size=block_size):
            progress_bar.update(len(chunk))
            if chunk:
                pip_py.write(chunk)
    subprocess.check_call([f"{embedded_path}/python.exe", "get-pip.py"])
    os.remove("get-pip.py")
    with open(f"python{major}{minor}._pth", "a") as include_file:
        include_file.write("\nLib\nLib/site-packages\n")
    os.chdir(cwd)


def installWithLib(embedded_path: str, package_name: str, *args) -> None:
    subprocess.check_call(
        f"pwsh -Command \"cp '{os.path.split(sys.executable)[0]}/include' '{embedded_path}/'\""
    )
    subprocess.check_call(
        f"pwsh -Command \"cp '{os.path.split(sys.executable)[0]}/libs' '{embedded_path}/'\""
    )
    subprocess.check_call(
        [f"{embedded_path}/python.exe", "-m", "pip", "install", package_name]
        + list(args)
    )
    shutil.rmtree(f"{embedded_path}/include")
    shutil.rmtree(f"{embedded_path}/libs")


def install(embedded_path: str, package_name: str, *args) -> None:
    subprocess.check_call(
        [f"{embedded_path}/python.exe", "-m", "pip", "install", package_name]
        + list(args)
    )


if __name__ == "__main__":
    dist = os.path.realpath(f"{os.path.split(__file__)[0]}/build/Release")
    if len(sys.argv) > 1:
        dist = os.path.realpath(sys.argv[1])

    print(f"Installing python to: {dist}")

    getEmbedded(dist)
    getPip(dist)
    install(dist, "packaging", "numpy", "Cython")
    installWithLib(dist, "tts")
    install(dist, "torch==1.11.0", "torchaudio==0.11.0")
