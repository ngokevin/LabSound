#!/bin/bash

xcodebuild -project osx/LabSound.xcodeproj -target LabSound -configuration Release
xcodebuild -project third_party/libnyquist/libnyquist.xcodeproj -target libnyquist -configuration Release