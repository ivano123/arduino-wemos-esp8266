#!/bin/bash -x


curl -X "POST" "https://api.github.com/repos/shaposhnikoff/arduino-wemos-esp8266/issues?state=all" \
     -H "Cookie: logged_in=no" \
     -H "Authorization: token $gh_token" \
     -H "Content-Type: text/plain; charset=utf-8" \
     -d $'{
  "title": "A workflow alerts Operator who performs a process",
  "body": "\\n${TRAVIS_JOB_ID}The \n\n\n\n\n command %s exited with ${result}.${TRAVIS_JOB_ID}\\n"
}'
