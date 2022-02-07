#!/usr/bin/bash

remote_name=epitech
repository_name=git@github.com:EpitechPromo2024/B-YEP-500-PAR-5-1-zia-arthur.fouquet.git

git ls-remote --exit-code ${remote_name} &> /dev/null
test 0 -eq $? || git remote add ${remote_name} ${repository_name}
git push ${remote_name} master:main
