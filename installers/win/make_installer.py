#!/usr/bin/env python3
"""Make the installer script for HyperTremolo"""
import xml.etree.ElementTree as ET
import contextlib
import functools
import argparse
import requests
import logging
import sys
import os
from typing import Sequence

logger = logging.getLogger("HyperTremolo Installer Builder")


class ReturnException(Exception):
  """Exception that represents the will to immediately exit"""
  value: int = 0


class ReturnFailException(ReturnException):
  """Exception that represents the will to immediately exit
  because of a failure"""
  value: int = 1


def exit_on_code(func):
  """Decorator that makes the program exit on a :cls:`ReturnException`"""

  @functools.wraps(func)
  def func_(*args, **kwargs):
    rv = s = None
    try:
      rv = func(*args, **kwargs)
    except ReturnException as e:
      rv = e.value
      s = str(e)
    if rv is not None:
      if rv == 0:
        logger.info("Done!")
      else:
        logger.error(s)
      sys.exit(rv)

  return func_


class ArgParser(argparse.ArgumentParser):
  """Argument parser for the installer options"""

  def __init__(self, *args, **kwargs):
    super().__init__(*args, **kwargs)
    root_dir = os.path.realpath(
        os.path.join(os.path.dirname(__file__), "..", ".."))
    self.add_argument("--jucer",
                      metavar="PATH",
                      default=os.path.join(root_dir, "HyperTremolo.jucer"),
                      help="Jucer project path")
    self.add_argument("-O",
                      "--output",
                      metavar="PATH",
                      default=None,
                      help="Output file path")
    self.add_argument("--license",
                      metavar="PATH",
                      default=os.path.join(root_dir, "LICENSE"),
                      help="License file path")
    self.add_argument("--icon",
                      metavar="PATH",
                      default=os.path.join(root_dir, "HyperTremolo.ico"),
                      help="Icon file path")
    self.add_argument("--pub-url",
                      metavar="URL",
                      default="https://www.lim.di.unimi.it",
                      help="Publisher URL")
    self.add_argument("--support-url",
                      metavar="URL",
                      default="https://github.com/"
                      "ChromaticIsobar/HyperTremolo/issues",
                      help="Support URL")
    self.add_argument("--updates-url",
                      metavar="URL",
                      default="https://github.com/"
                      "ChromaticIsobar/HyperTremolo/releases",
                      help="Updates URL")
    self.add_argument("--icon-url",
                      metavar="URL",
                      default="https://gist.githubusercontent.com"
                      "/ChromaticIsobar/6ce8126b4bc9a02594282bde2909f02b"
                      "/raw/HyperTremolo.ico",
                      help="Icon file URL")
    self.add_argument("--vst3-64-path",
                      metavar="PATH",
                      default=None,
                      help="Path of the x64 VST3 plugin")
    self.add_argument("--vst3-32-path",
                      metavar="PATH",
                      default=None,
                      help="Path of the Win32 VST3 plugin")
    self.add_argument("-q",
                      "--quiet",
                      dest="log_level",
                      action="store_const",
                      const=logging.ERROR,
                      default=logging.INFO,
                      help="Log only error messages")
    self.add_argument("-v",
                      "--verbose",
                      dest="log_level",
                      action="store_const",
                      const=logging.DEBUG,
                      default=logging.INFO,
                      help="Log debug messages too")

  def parse_args(self, argv: Sequence[str]) -> argparse.Namespace:
    """Parse CLI arguments

    Args:
      argv (sequence of str): CLI arguments

    Returns:
      Namespace: Parsed arguments"""
    args = super().parse_args(argv)

    # Set up logger
    logging.basicConfig(
        level=args.log_level,
        format="%(asctime)s %(name)-12s %(levelname)-8s: %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S")

    return args


def download_to_file(file, url: str, chunk_size: int = 1 << 13) -> int:
  """Download file from URL

  Args:
    file: Writable stream
    url (str): URL of the file to download
    chunk_size (int): Chunk size for download stream

  Returns:
    int: Status code"""
  with requests.get(url, stream=True) as r:
    r.raise_for_status()
    for c in r.iter_content(chunk_size=chunk_size):
      file.write(c)
    return r.status_code


@contextlib.contextmanager
def open_or_stdout(fname=None, mode="w", encoding="utf-8"):
  """Open a file or stdout

  Args:
    fname (str): File path. If :data:`None` (default), yield stdout
    mode: Mode for file open
    encoding: File encoding

  Yields:
    File handler for the file or stdout"""
  if fname is None:
    yield sys.stdout
  else:
    with open(fname, mode=mode, encoding=encoding) as f:
      yield f


def read_jucer(args: argparse.Namespace) -> ET.Element:
  """Read the jucer project XML file

  Args:
    args (Namespace): CLI arguments

  Returns:
    Element: XML root element"""
  tree = ET.parse(args.jucer)
  return tree.getroot()


def section_from_file(fname: str, relpath: bool = True, text_var: str = "text"):
  """Make a decorator for providing a default argument from a text file

  Args:
    fname (str): Name of the file to be read
    relpath (bool): If :data:`True` (default), the file name is interpreted
      as a relative path, otherwise as an absolute path
    text_var (str): Name of the variable to pass to the decorated function

  Returns:
    The decorator function"""

  @functools.wraps(section_from_file)
  def section_from_file_(func, fname=fname):
    """Decorator function"""
    if relpath:
      fname = os.path.realpath(os.path.join(os.path.dirname(__file__), fname))
    with open(fname, mode="r", encoding="utf-8") as f:
      # Decorated function
      return functools.partial(func, **{text_var: f.read()})

  return section_from_file_


def download_icon(args: argparse.Namespace):
  """Download the icon file

  Args:
    args (Namespace): CLI arguments"""
  if args.icon:
    if os.path.isfile(args.icon):
      logger.debug("Skipping icon download. File found: %s", args.icon)
    elif not args.icon_url:
      logger.debug("Skipping icon download. No URL specified")
    else:
      logger.info("Downloading icon from: %s", args.icon_url)
      logger.debug("Downloading icon to:   %s", args.icon)
      with open(args.icon, mode="wb") as f:
        download_to_file(f, args.icon_url)
  else:
    logger.debug("Not downloading any icon")


@section_from_file("Setup.txt")
def write_setup(jucer: ET.Element,
                args: argparse.Namespace,
                text: str = "") -> str:
  """Write the [Setup] portion of the installer script

  Args:
    jucer (Element): Jucer project XML root
    args (Namespace): CLI arguments
    text (str): Prototype of the [Setup] portion to format

  Returns:
    str: The formatted [Setup] portion of the installer script"""
  return text.format(
      q="\"",
      lb="{",
      rb="}",
      bs="\\",
      name=jucer.attrib["name"],
      version=jucer.attrib["version"],
      publisher=jucer.attrib["pluginManufacturer"],
      company=jucer.attrib["companyName"],
      pub_url=args.pub_url,
      support_url=args.support_url,
      updates_url=args.updates_url,
      icon=args.icon,
      license=args.license,
  )


@section_from_file("Types.txt")
def write_types(text: str = "") -> str:
  """Write the [Types] portion of the installer script

  Args:
    text (str): Prototype of the [Types] portion to format

  Returns:
    str: The formatted [Types] portion of the installer script"""
  return text


@section_from_file("File.txt", text_var="file")
@section_from_file("Component.txt", text_var="component")
def write_components_files(args: argparse.Namespace,
                           component: str = "",
                           file: str = "") -> str:
  """Write the [Components] and [Files] portions of the installer script

  Args:
    args (Namespace): CLI arguments
    component (str): Prototype of a component row to format
    file (str): Prototype of a file row to format

  Returns:
    str: The [Components] and [Files] portions of the installer script"""
  r = ["[Components]"]
  if args.vst3_64_path is not None:
    r.append(component.format(name="VST3_64", description="VST3 Plugin 64-bit"))
  if args.vst3_32_path is not None:
    r.append(component.format(name="VST3_32", description="VST3 Plugin 32-bit"))
  if len(r) < 2:
    raise ReturnFailException("No Plugin file specified")
  r.extend(("", "[Files]"))
  if args.vst3_64_path is not None:
    r.append(
        file.format(name="VST3_64",
                    path=os.path.realpath(args.vst3_64_path),
                    lb="{",
                    rb="}"))
  if args.vst3_32_path is not None:
    r.append(
        file.format(name="VST3_32",
                    path=os.path.realpath(args.vst3_32_path),
                    lb="{",
                    rb="}"))
  r.append("")
  return "\n".join(r)


@section_from_file("Icons.txt")
def write_icons(jucer: ET.Element, text: str = "") -> str:
  """Write the [Icons] portion of the installer script

  Args:
    jucer (Element): Jucer project XML root
    text (str): Prototype of the [Icons] portion to format

  Returns:
    str: The formatted [Icons] portion of the installer script"""
  return text.format(
      lb="{",
      rb="}",
      name=jucer.attrib["name"],
  )


@section_from_file("Code.txt")
def write_code(text: str = "") -> str:
  """Write the [Code] portion of the installer script

  Args:
    text (str): Prototype of the [Code] portion to format

  Returns:
    str: The formatted [Code] portion of the installer script"""
  return text


@exit_on_code
def main(*argv):
  """Main script"""
  args = ArgParser(description=__doc__).parse_args(argv)

  jucer = read_jucer(args)
  download_icon(args)
  with open_or_stdout(args.output) as outf:
    outf.write(write_setup(jucer, args))
    outf.write("\n")
    outf.write(write_types())
    outf.write("\n")
    outf.write(write_components_files(args))
    outf.write("\n")
    outf.write(write_icons(jucer))
    outf.write("\n")
    outf.write(write_code())

  return 0


if __name__ == "__main__":
  main(*sys.argv[1:])
