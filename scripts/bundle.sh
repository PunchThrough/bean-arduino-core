#!/usr/bin/env bash

BUNDLE_NAME="bean-arduino-core"
BUNDLE_PATH="tmp/${BUNDLE_NAME}"

rm -rf "${BUNDLE_PATH}"
mkdir -p "${BUNDLE_PATH}/hardware/LightBlue-Bean"
mkdir -p "${BUNDLE_PATH}/examples/LightBlue-Bean"

cp -R "hardware/bean/" "${BUNDLE_PATH}/hardware/LightBlue-Bean"
cp -R "examples/" "${BUNDLE_PATH}/examples/LightBlue-Bean"

GITINFO=$(git describe)
tar -zcvf "${BUNDLE_NAME}-${GITINFO}.tar.gz" "${BUNDLE_PATH}"
