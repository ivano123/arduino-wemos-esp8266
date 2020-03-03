#!/bin/bash 


#export TRAVIS_JOB_ID=10101010101
#export TRAVIS_BUILD_WEB_URL="https://kjsdhfkjshdkfjshdfkjhs.com"
#export TRAVIS_BUILD_WEB_URL="https://kjsdhfjkshdfkjshf.com"
#export TRAVIS_JOB_WEB_URL="https://jksdhfksdhfjksdhfk.sfsfsdf.sdfsfsdfs.ru"

echo $TRAVIS_JOB_ID




BUCKET_NAME=testbucket
OBJECT_NAME=testworkflow-2.0.1.jar
TARGET_LOCATION=/opt/test/testworkflow-2.0.1.jar

JSON_STRING=$( jq -n \
                  --arg bn "$BUCKET_NAME" \
                  --arg on "$OBJECT_NAME" \
                  --arg tl "$TARGET_LOCATION" \
                  '{bucketname: $bn, objectname: $on, targetlocation: $tl}' )

#echo $JSON_STRING

curl -X "POST" "https://api.github.com/repos/shaposhnikoff/arduino-wemos-esp8266/issues?state=all" \
     -H "Cookie: logged_in=no" \
     -H "Authorization: token $gh_token" \
     -H "Content-Type: text/plain; charset=utf-8" \
     -H "Accept: application/vnd.github.v3.text+json" \
     -d $'{
  "title": "Alert '${PLATFORMIO_CI_SRC}'",
  "labels": ["bug"],
  "body": "Job id is '${TRAVIS_JOB_ID}' \r\n Build was failed with ${result}. \r\n travis job id - '${TRAVIS_JOB_ID}' \r\n\n travis build web url \r\n '${TRAVIS_BUILD_WEB_URL}' \r\n\n  travis job web url \r\n '${TRAVIS_JOB_WEB_URL}' \r\n  raw log url  \r\n  '${RAW_LOG_URL}'"
}'

