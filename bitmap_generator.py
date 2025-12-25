Import("projenv")
# extra post action for converting face images from png to source code

try:
    from PIL import Image
except ImportError:
    projenv.Execute(
        projenv.VerboseAction(
            '$PYTHONEXE -m pip install "pillow==12.0.0" ',
            "Installing build Python dependencies",
        )
    )
    from PIL import Image

import os
import re
import json
from datetime import datetime
from math import floor

class Bitmap:
    def __init__(self, name, data, size):
        self.name = name  
        self.data = data
        self.size = size

def matrix_to_point(bit, row):
    offset = floor(row / 8) * 8
    return ((7 - bit + offset), (row - offset))

def process_image(image):
    width, height = image.size
    ret = []
    for row in range(0, width):
        result = 0
        for bit in range(7, -1, -1):
            point = matrix_to_point(bit, row)
            data = image.getpixel(point)
            result = (result << 1) | (data & 1)
        ret.append(result)
    return ret

def str_to_identifier(string):
    return "".join(
        re.findall(
            "[_a-zA-Z][_a-zA-Z0-9]*", 
            string
        )
    )

bitmap_file_types = [".png", ".bmp"]
bitmaps_dir = "src/Assets/Bitmaps"
source_filename = "Standard"
bitmaps_header_location = os.path.join(bitmaps_dir, f"{source_filename}.h")
bitmaps_source_location = os.path.join(bitmaps_dir, f"{source_filename}.cpp")

# iterate over bitmap asset directories, note down unique folders as namespaces, note down latest modify time for later comparison
namespaces = []
last_mtime = 0

for root, dirs, files in os.walk(bitmaps_dir):
    namespaces += dirs
    for file in files:
        if os.path.splitext(file)[1] not in bitmap_file_types:
            continue
        path = os.path.join(root, file)
        mtime = os.path.getmtime(path)
        if mtime > last_mtime:
            last_mtime = mtime

if not (os.path.exists(bitmaps_source_location) and os.path.exists(bitmaps_header_location) and os.path.getmtime(bitmaps_source_location) > last_mtime and os.path.getmtime(bitmaps_header_location) > last_mtime):
    print("Bitmap assets have been updated, regenerating code for bitmaps...")
    print(f"Found {len(namespaces)} namespaces: {', '.join(namespaces)}")

    result = {n : [] for n in namespaces}
    metadata = {}

    # main code generator loop
    for root, dirs, files in os.walk(bitmaps_dir):
        namespace = os.path.basename(root)
        for file in files:
            file_extension = os.path.splitext(file)[1]
            if file == "metadata.json":
                # load metadata
                with open(os.path.join(root, file)) as metadata_file:
                    metadata[namespace] = json.load(metadata_file)
                continue

            if os.path.splitext(file)[1] not in bitmap_file_types:
                continue

            path = os.path.join(root, file)
            image_name = str_to_identifier("".join(file.split(".")[:-1]))

            im = Image.open(path)
            im = im.convert("1", dither=Image.Dither.NONE)
            converted = process_image(im)
            print(f"{file} {im.size} -> Bitmaps::{namespace}::{image_name}")
            result[namespace].append( 
                Bitmap (
                    name = image_name,
                    data = ", ".join(map(lambda x: format(x, "#04x"), converted)),
                    size = im.size[0]
                )
            )

    timestamp = datetime.now()

    # sort namespaces and bitmaps alphabetically
    for namespace in result:
        result[namespace].sort(key = lambda x: x.name)
    result = dict(sorted(result.items()))

    # parse metadata
    print("Parsing metadata...")
    for namespace in metadata.keys():
        metadata[namespace]["randomizerExceptions"] = list(map(lambda filename: str_to_identifier("".join(filename.split(".")[:-1])), metadata[namespace]["randomizerExceptions"]))

    print("Generating bitmap code...")

    # generate and save header file
    out = f"""#pragma once
// This header file is automatically generated during the build process!
// To modify, edit the original png files and build again.

#include <vector>
#include "Arduino.h"
#include "Typedefs.h"

namespace Bitmaps {{
    extern const char* BITMAP_GENERATION_TIMESTAMP;\n
"""

    for namespace in result:
        out += f"    namespace {namespace} {{\n        extern Bitmap\n"
        out += ",\n".join(f"            {bitmap.name}[{bitmap.size}]" for bitmap in result[namespace])
        out += ";\n    }\n\n"

    out += "    extern std::vector<Bitmap*>\n"
    out += ",\n".join(f"        {namespace}Bitmaps" for namespace in result)
    out += ";\n"

    out += "}"

    with open(bitmaps_header_location, "w") as output_file:
        output_file.write(out)

    # generate and save cpp file
    out = f"""#include \"{source_filename}.h\"
// This source file is automatically generated during the build process!
// To modify, edit the original png files and build again.

namespace Bitmaps {{
    const char* BITMAP_GENERATION_TIMESTAMP = \"{timestamp}\";\n
"""

    for namespace in result:
        out += f"    namespace {namespace} {{\n"
        for bitmap in result[namespace]:
            out += f"        Bitmap {bitmap.name}[{bitmap.size}] {{ {bitmap.data} }};\n"
        out += "    }\n\n"

    for namespace in result:
        out += f"    std::vector<Bitmap*> {namespace}Bitmaps {{\n"
        out += ",\n".join(f"        {namespace}::{bitmap.name}" for bitmap in filter(lambda bitmap: bitmap.name not in metadata[namespace]["randomizerExceptions"], result[namespace]))
        out += "\n    };\n\n"

    out += "}"

    with open(bitmaps_source_location, "w") as output_file:
        output_file.write(out)

    print("Finished bitmap code generation.")

else:
    print("No changes in bitmaps, skipping bitmap code generation.")