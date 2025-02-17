/*
 * Copyright (C) 2021 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "FingerprintInscreenService"

#include <android-base/file.h>
#include "FingerprintInscreen.h"

#include <hidl/HidlTransportSupport.h>

#define CMD_FINGER_DOWN 200001
#define CMD_FINGER_UP 200003
#define CMD_LIGHT_AREA_CLOSE 200000
#define CMD_LIGHT_AREA_STABLE 200002
#define CMD_PARTIAL_FINGER_DETECTED 200004

#define GLOBAL_HBM_PATH "/proc/globalHbm"
#define GLOBAL_HBM_ON "1"
#define GLOBAL_HBM_OFF "0"

namespace vendor {
namespace lineage {
namespace biometrics {
namespace fingerprint {
namespace inscreen {
namespace V1_0 {
namespace implementation {

FingerprintInscreen::FingerprintInscreen() {
    this->mGoodixFingerprintDaemon = IGoodixFingerprintDaemon::getService();
}

Return<void> FingerprintInscreen::onStartEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::onFinishEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::onPress() {
    this->mGoodixFingerprintDaemon->sendCommand(CMD_FINGER_DOWN, {},
                                                [](int, const hidl_vec<signed char>&) {});
    if (!android::base::WriteStringToFile(GLOBAL_HBM_ON, GLOBAL_HBM_PATH)) {
        ALOGE("Failed to write to %s", GLOBAL_HBM_PATH);
    }
    this->mGoodixFingerprintDaemon->sendCommand(CMD_LIGHT_AREA_STABLE, {},
                                                [](int, const hidl_vec<signed char>&) {});
    return Void();
}

Return<void> FingerprintInscreen::onRelease() {
    if (!android::base::WriteStringToFile(GLOBAL_HBM_OFF, GLOBAL_HBM_PATH)) {
        ALOGE("Failed to write to %s", GLOBAL_HBM_PATH);
    }
    this->mGoodixFingerprintDaemon->sendCommand(CMD_FINGER_UP, {},
                                                [](int, const hidl_vec<signed char>&) {});
    return Void();
}

Return<void> FingerprintInscreen::onShowFODView() {
    return Void();
}

Return<void> FingerprintInscreen::onHideFODView() {
    android::base::WriteStringToFile(GLOBAL_HBM_OFF, GLOBAL_HBM_PATH);
    return Void();
}

Return<bool> FingerprintInscreen::handleAcquired(int32_t, int32_t) {
    return false;
}

Return<bool> FingerprintInscreen::handleError(int32_t, int32_t) {
    return false;
}

Return<void> FingerprintInscreen::setLongPressEnabled(bool) {
    return Void();
}

Return<int32_t> FingerprintInscreen::getDimAmount(int32_t) {
    return 0;
}

Return<bool> FingerprintInscreen::shouldBoostBrightness() {
    return false;
}

Return<void> FingerprintInscreen::setCallback(const sp<IFingerprintInscreenCallback>&) {
    return Void();
}

Return<int32_t> FingerprintInscreen::getPositionX() {
    return 435;
}

Return<int32_t> FingerprintInscreen::getPositionY() {
    return 1631;
}

Return<int32_t> FingerprintInscreen::getSize() {
    return 220;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace inscreen
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace lineage
}  // namespace vendor
