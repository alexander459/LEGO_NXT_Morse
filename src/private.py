#!/usr/bin/env python3

###################################################
#
# file: private.py
#
# @Author:   Iacovos G. Kolokasis
#
# Change the visibility of the forked project to
# private and add TAs as members with developer
# role in the project
#
###################################################

import gitlab
import optparse

usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage=usage)
parser.add_option("-t", "--token", dest="token",
                  help="Pesonal token authentication")
parser.add_option("-p", "--projectId", dest="project_id", help="Project Id")
(options, args) = parser.parse_args()

# TAs ids
ta_id = [723]

# Personal token authentication
token = options.token

# Project Id
project_id = options.project_id

gl = gitlab.Gitlab('https://gitlab-csd.datacenter.uoc.gr', private_token=token)
gl.auth()

project = gl.projects.get(project_id)

# Change visibility of forked projects from public to private
project.visibility = 'private'
project.save()
print("Project: " + project.name + " is " + project.visibility)

# Add TAs as developer members to the project
for tid in ta_id:
    member = project.members.create(
        {'user_id': tid, 'access_level': gitlab.DEVELOPER_ACCESS})
    project.save()
    print("User: " + member.name + " is member to " + project.name)
