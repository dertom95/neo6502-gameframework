import glob
import re
import xml.etree.ElementTree as ET
import subprocess
import xml.dom.minidom
import re

def simple_glue(input_paths,output_file):
    filepaths = []
    for path in input_paths:
        if '*' in path:
            filepaths.extend(glob.glob(path))
        else:
            filepaths.append(path)

    # create root element
    _root = ET.Element("root")
    root = ET.SubElement(_root,"gluecode")

    lines=[]
    # read c header file
    for filename in filepaths:
        with open(filename, "r") as f:
            lines.extend(f.readlines())


    function_groups={}
    for line in lines:
        # check if the line starts with /*api*/
        if line.startswith("/*api"):
            # use regex to match the function-group, function-id and the rest of the line
            match = re.match(r"/\*api:(\d+):(\d+)\*/(.*)", line)
            if match:
                # get the function-group, function-id and the rest of the line
                function_group = match.group(1)
                function_id = match.group(2)
                line = match.group(3).strip()
                if function_group not in function_groups:
                    xfunction_group = ET.SubElement(root,"function-group")
                    function_groups[function_group]=xfunction_group
                    xfunction_group.set("id",function_group)
                else:
                    xfunction_group = function_groups[function_group]

                # use regex to match the return type, function name and parameters
                match = re.match(r"(\w+)\s+(\w+)\((.*)\);", line)
                if match:
                    # get the return type, function name and parameters
                    return_type = match.group(1)
                    function_name = match.group(2)
                    parameters = match.group(3).split(",")
                    # create function element
                    function = ET.SubElement(xfunction_group, "function")
                    # set the name, return_type, function_group and function_id attributes
                    function.set("name", function_name)
                    function.set("return_type", return_type)
                    function.set("function_id", function_id)
                    first, rest = line.split(" ", maxsplit=1)
                    function.set("signature", rest.rstrip("; "))
                    # create parameters element
                    # parameters_element = ET.SubElement(function, "parameters")
                    # loop through the parameters
                    for parameter in parameters:
                        # use regex to match the parameter type and name
                        match = re.match(r"(\w+)\s+(\w+)", parameter.strip())
                        if match:
                            # get the parameter type and name
                            parameter_type = match.group(1)
                            parameter_name = match.group(2)
                            # create parameter element
                            parameter_element = ET.SubElement(function, "parameter")
                            # set the name and type attributes
                            parameter_element.set("name", parameter_name)
                            parameter_element.set("type", parameter_type)


    # write the xml to a file
    tree = ET.ElementTree(_root)
    tree.write(output_file, encoding="utf-8", xml_declaration=True)

def create_gluecode_xml(args):
    simple_glue(args.filepaths, args.output)

 