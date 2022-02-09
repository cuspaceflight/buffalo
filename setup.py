from setuptools import setup, Extension
from platform import system

from glob import glob

buffalo = Extension(
    "buffalo",
    sources=[
        "src/state_estimation.c",
        "src/buffalo.c"
    ],
    include_dirs=["include"],
    extra_compile_args=["-lm"]
)

setup(
    name="buffalo",
    version="1.0",
    description="Flight computer simulation.",
    author="H. E. Franks",
    author_email="henry@cusf.co.uk",
    ext_modules=[buffalo]
)

