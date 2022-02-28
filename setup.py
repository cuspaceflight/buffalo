from setuptools import setup, Extension
from platform import system

from glob import glob

buffalo = Extension(
    "simulation.buffalo",
    sources=glob("src/*.c"),
    include_dirs=["include"],
    extra_compile_args=["-lm"]
)

setup(
    name="buffalo",
    version="1.0",
    description="Flight computer simulation.",
    author="H. E. Franks",
    author_email="hef36@cam.ac.uk",
    ext_modules=[buffalo]
)

