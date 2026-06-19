#!/usr/bin/env python3

import os
import re
from pathlib import Path
from tempfile import NamedTemporaryFile


def get_build_number():
    """Get and validate the build number."""
    build_num = os.getenv("BuildNum")

    if not build_num:
        raise ValueError("Environment variable 'BuildNum' is not set.")

    if not build_num.isdigit():
        raise ValueError(
            f"Invalid BuildNum '{build_num}'. It must contain only digits."
        )

    return build_num


def get_source_directory():
    """Get and validate the source directory."""
    source_path = os.getenv("SourcePath")

    if not source_path:
        raise ValueError("Environment variable 'SourcePath' is not set.")

    src_dir = Path(source_path) / "develop" / "global" / "src"

    if not src_dir.is_dir():
        raise FileNotFoundError(f"Source directory does not exist: {src_dir}")

    return src_dir


def update_file(file_path, pattern, replacement):
    """
    Update exactly one occurrence of a pattern in a file.
    """

    replaced = False

    with open(file_path, "r", encoding="utf-8") as src:
        with NamedTemporaryFile(
            mode="w",
            delete=False,
            encoding="utf-8",
            dir=file_path.parent,
        ) as temp:

            for line in src:
                new_line, count = re.subn(
                    pattern,
                    replacement,
                    line,
                    count=1,
                )

                if count:
                    if replaced:
                        raise RuntimeError(
                            f"Multiple matching lines found in {file_path}"
                        )
                    replaced = True

                temp.write(new_line)

    if not replaced:
        os.unlink(temp.name)
        raise RuntimeError(
            f"Pattern '{pattern}' not found in {file_path}"
        )

    os.replace(temp.name, file_path)


def main():
    build_num = get_build_number()
    src_dir = get_source_directory()

    update_file(
        src_dir / "SConstruct",
        r"point\s*=\s*\d+",
        f"point={build_num}",
    )

    update_file(
        src_dir / "VERSION",
        r"ADLMSDK_VERSION_POINT\s*=\s*\d+",
        f"ADLMSDK_VERSION_POINT={build_num}",
    )

    print(f"Build number successfully updated to {build_num}.")


if __name__ == "__main__":
    main()