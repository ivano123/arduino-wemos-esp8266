#!/bin/bash -x


#export TRAVIS_JOB_ID=10101010101
#export TRAVIS_BUILD_WEB_URL="https://kjsdhfkjshdkfjshdfkjhs.com"
echo $TRAVIS_JOB_ID

curl -X "POST" "https://api.github.com/repos/shaposhnikoff/arduino-wemos-esp8266/issues?state=all" \
     -H "Cookie: logged_in=no" \
     -H "Authorization: token $gh_token" \
     -H "Content-Type: text/plain; charset=utf-8" \
     -d $'{
  "title": "A workflow alerts Operator who performs a process '${TRAVIS_JOB_ID}'",
  "body": "'${TRAVIS_JOB_ID}' The command %s exited with ${result}.'${TRAVIS_JOB_ID}' travis build web url  '${TRAVIS_BUILD_WEB_URL}'  travis job web url  '${TRAVIS_JOB_WEB_URL}'"
}'
