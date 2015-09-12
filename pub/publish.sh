#!/bin/bash
# Prepare appinfo.json for publishing
# RUN FROM THE APPINFO.JSON DIRECTORY!!

# Load UUIDs
pub_id=$(cat pub/uuid | grep publish | cut -f 2 -d:)
test_id=$(cat pub/uuid | grep test | cut -f 2 -d:)
# Replace UUID
sed -i '' "s/$test_id/$pub_id/g" appinfo.json

echo "UUID switched to" $pub_id
