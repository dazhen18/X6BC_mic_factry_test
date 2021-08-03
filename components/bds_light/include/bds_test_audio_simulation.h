/*
 * bds_test_audio_simulation.h
 *
 *  Created on: May 13, 2020
 *      Author: liweigao
 */

#ifndef LIGHT_BDSPEECH_SDK_COMPONENTS_BDS_LIGHT_SDK_INCLUDE_BDS_TEST_AUDIO_SIMULATION_H_
#define LIGHT_BDSPEECH_SDK_COMPONENTS_BDS_LIGHT_SDK_INCLUDE_BDS_TEST_AUDIO_SIMULATION_H_

#define KEY_NAME        "name"
#define KEY_FILE_NAME       "file_name"

typedef void*   bds_as_handle_t;
typedef void (*bds_as_cb_f)(char *buffer);

bds_as_handle_t bds_as_create();

void bds_as_scan_file(bds_as_handle_t handle, const char *dir, bds_as_cb_f callback);

int bds_as_simulate(bds_as_handle_t handle, const char *file_path);

int bds_as_get_frame_count(bds_as_handle_t handle);

void bds_as_destroy(bds_as_handle_t handle);

#endif /* LIGHT_BDSPEECH_SDK_COMPONENTS_BDS_LIGHT_SDK_INCLUDE_BDS_TEST_AUDIO_SIMULATION_H_ */
