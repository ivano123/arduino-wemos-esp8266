#!/bin/bash -x


curl -X "POST" "https://api.github.com/repos/shaposhnikoff/arduino-wemos-esp8266/issues?state=all" \
     -H "Cookie: logged_in=no" \
     -H "Authorization: token $gh_token" \
     -H "Content-Type: text/plain; charset=utf-8" \
     -d $'{
  "title": "A workflow alerts Operator who performs a process",
  "body": "${TRAVIS_WHEREAMI_URL:-https://whereami.travis-ci.com}"
}'
