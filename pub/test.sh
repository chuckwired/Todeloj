#!/bin/bash
# Prepare appinfo.json for testing
# RUN FROM THE APPINFO.JSON DIRECTORY!!

# Load UUIDs
pub_id=$(cat pub/uuid | grep publish | cut -f 2 -d:)
test_id=$(cat pub/uuid | grep test | cut -f 2 -d:)
# Replace UUID
sed -i '' "s/$pub_id/$test_id/g" appinfo.json

echo "UUID switched to" $test_id
