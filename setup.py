from setuptools import setup, Extension
from pathlib import Path

buffalo = Extension(
    "simulation.buffalo",
    sources=[path.as_posix() for path in Path("src").rglob("*.c")],
    include_dirs=["include"],
    extra_compile_args=["-lmg"]
)

setup(
    name="buffalo",
    version="1.0",
    description="Flight computer simulation.",
    author="H. E. Franks",
    author_email="hef36@cam.ac.uk",
    ext_modules=[buffalo]
)

