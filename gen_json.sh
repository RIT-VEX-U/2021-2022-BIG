#!/bin/sh

# vision files need the "specialType" of "vision_config"
# robot config files need the "specialType" of "device_config"

# Search for source/header files, excluding those in the .git directory
files=$(find -name "*.cpp" |grep -v "git" |sed "s|./||")
files="${files} $(find -name "*.h" |grep -v "git" |sed "s|./||")"
files="${files} $(find -name "*.md" |grep -v "git" |sed "s|./||")"

# Search for directories under src, include or core
directories=$(find -type d -regex "./\(core\|src\|include\)/.*" |sed "s|./||")

output=$(mktemp)

# generate the "File" type of json
for i in ${files}; do
	builder="{\"name\": \"$i\", \"type\": \"File\", \"specialType\":\"\"},"
	echo "${builder}" >> $output
done

# generate the "Folder" type of json
for i in ${directories}; do
	builder="{\"name\": \"$i\", \"type\": \"Directory\"},"
	echo "${builder}" >> $output
done

sed -i "$ s/,$//" $output

cat $output
