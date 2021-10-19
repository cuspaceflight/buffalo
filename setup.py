from setuptools import setup, Extension
from platform import system

from glob import glob

buffalo = Extension(
    "buffalo",
    sources=glob("src/*.c"),
    include_dirs=["include"]
)

setup(
    name="buffalo",
    version="1.0",
    description="Flight computer simulation.",
    author="H. E. Franks",
    author_email="henry@cusf.co.uk",
    ext_modules=[buffalo]
)

