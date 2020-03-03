#!/bin/bash -x


#export TRAVIS_JOB_ID=10101010101
#export TRAVIS_BUILD_WEB_URL="https://kjsdhfkjshdkfjshdfkjhs.com"
#export TRAVIS_BUILD_WEB_URL="https://kjsdhfjkshdfkjshf.com"
#export TRAVIS_JOB_WEB_URL="https://jksdhfksdhfjksdhfk.sfsfsdf.sdfsfsdfs.ru"


echo $TRAVIS_JOB_ID

curl -X "POST" "https://api.github.com/repos/shaposhnikoff/arduino-wemos-esp8266/issues?state=all" \
     -H "Cookie: logged_in=no" \
     -H "Authorization: token $gh_token" \
     -H "Content-Type: text/plain; charset=utf-8" \
     -d $'{
  "title": "Alert '${PLATFORMIO_CI_SRC}'",
  "body": "Job id is '${TRAVIS_JOB_ID}' \r\n The command %s exited with ${result}. \r\n travis job id - '${TRAVIS_JOB_ID}' \r\n travis build web url  '${TRAVIS_BUILD_WEB_URL}' \r\n  travis job web url  '${TRAVIS_JOB_WEB_URL}'"
}'
