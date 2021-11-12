#!/usr/bin/env python3
"""Download and install HyperTremolo"""
import functools
import tempfile
import argparse
import requests
import logging
import zipfile
import shutil
import json
import stat
import sys
import os
from typing import Sequence

logger = logging.getLogger("HyperTremolo Installer")


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
    self.add_argument("--release-endpoint",
                      metavar="URL",
                      default="https://api.github.com/repos/ChromaticIsobar/"
                      "HyperTremolo/releases",
                      help="Releases API endpoint URL")
    self.add_argument("--prefix",
                      metavar="PATH",
                      default=None,
                      help="Installation prefix path")
    self.add_argument("-U",
                      "--user",
                      action="store_const",
                      const=True,
                      default=True,
                      help="Install under user directory (default)")
    self.add_argument("-G",
                      "--global",
                      dest="user",
                      action="store_const",
                      const=False,
                      default=True,
                      help="Install under system directory")
    self.add_argument("--standalone",
                      action="store_true",
                      help="Install standalone")
    self.add_argument("--vst3",
                      dest="standalone",
                      action="store_const",
                      const=False,
                      default=False,
                      help="Install VST3 (default)")
    self.add_argument("-V", "--version", default=None, help="Version number")
    self.add_argument("-q",
                      "--quiet",
                      dest="log_level",
                      action="store_const",
                      const=logging.ERROR,
                      default=logging.INFO,
                      help="Log only error messages")
    self.add_argument("--list",
                      action="store_true",
                      help="List available releases and quit")
    self.add_argument("--uninstall",
                      action="store_true",
                      help="Uninstall the software")
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
      Namespace, int: Parsed arguments and exit code (or :data:`None`)"""
    args = super().parse_args(argv)

    # Set up logger
    logging.basicConfig(
        level=args.log_level,
        format="%(asctime)s %(name)-12s %(levelname)-8s: %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S")

    if args.standalone:
      # Settings for standalone executble
      args.asset_name_fn = lambda release:"HyperTremolo_linux_standalone_" \
                                         f"{release['tag_name']}.zip"
      if args.prefix is None:
        if args.user:
          args.prefix = os.path.expanduser("~/.local/bin")
        else:
          args.prefix = os.path.expanduser("/usr/bin")
      args.dest_path = os.path.join(args.prefix, "HyperTremolo")
    else:
      # Settings for VST3 plugin
      args.asset_name_fn = lambda release: "HyperTremolo_linux_vst3_" \
                                          f"{release['tag_name']}.zip"
      if args.prefix is None:
        if args.user:
          args.prefix = os.path.expanduser("~/.vst3")
        else:
          raise ReturnFailException(
              "No default system-wide path defined for VST3 plugins. "
              "Please, specify an installation prefix")
      args.dest_path = os.path.join(args.prefix, "HyperTremolo.vst3")
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


def uninstall(args: argparse.Namespace):
  """Uninstall script

  Args:
    args (Namespace): CLI arguments"""
  if not args.uninstall:
    return
  logger.info("Uninstalling HyperTremolo: %s", args.dest_path)
  if args.standalone:
    if not os.path.isfile(args.dest_path):
      raise ReturnFailException(f"File not found: {args.dest_path}")
    os.remove(args.dest_path)
  else:
    if not os.path.isdir(args.dest_path):
      raise ReturnFailException(f"File not found: {args.dest_path}")
    shutil.rmtree(args.dest_path)
  raise ReturnException()


def check_prefix_exists(args: argparse.Namespace):
  """Check that the installation prefix exists

  Args:
    args (Namespace): CLI arguments"""
  if not os.path.isdir(args.prefix):
    raise ReturnFailException(
        f"Installation prefix does not exist: '{args.prefix}'. "
        "Please, check that the path is correct and, "
        "eventually, create it")
  else:
    if not args.list:
      logger.info("Installing HyperTremolo: %s", args.dest_path)


def lookup(args: argparse.Namespace) -> str:
  """Look for a release asset that matches the requirements

  Args:
    args (Namespace): CLI arguments

  Returns:
    str: The asset download URL"""
  # Get releases information
  releases = requests.get(args.release_endpoint,
                          headers={"Accept": "application/vnd.github.v3+json"})
  releases = json.loads(releases.text)

  # Look for matching assets
  r = a = None
  for r in releases:
    if args.list:
      print(r["tag_name"][1:])
      continue
    logger.debug("Reading release %s", r["name"])
    if args.version is not None and r["tag_name"] != f"v{args.version}":
      logger.debug("Skip for version constraint")
      continue
    name = args.asset_name_fn(r)
    for a in r["assets"]:
      if a["name"] == name:
        break
    else:
      logger.debug("Asset '%s' not found", name)
      continue
    break
  else:
    if args.list:
      raise ReturnException()
    raise ReturnFailException("No matching asset found")
  logger.info("Installing HyperTremolo %s", r["tag_name"])
  logger.debug("Asset URL:            %s", a["browser_download_url"])
  return a["browser_download_url"]


def download_and_install(args: argparse.Namespace, url: str):
  """Download the asset and install it

  Args:
    args (Namespace): CLI arguments
    url (str): The asset download URL"""
  with tempfile.TemporaryFile() as zip_file_w:
    download_to_file(zip_file_w, url)
    with zipfile.ZipFile(zip_file_w, mode="r") as zip_file_r:
      zip_file_r.extractall(args.prefix)
    if args.standalone:
      logger.debug("Setting executable permissions")
      s = os.stat(args.dest_path).st_mode
      s |= stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH
      os.chmod(args.dest_path, s)


@exit_on_code
def main(*argv):
  """Main script"""
  args = ArgParser(description=__doc__).parse_args(argv)

  uninstall(args)
  check_prefix_exists(args)
  download_and_install(args, lookup(args))

  return 0


if __name__ == "__main__":
  main(*sys.argv[1:])
