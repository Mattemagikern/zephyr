# Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
# SPDX-License-Identifier: Apache-2.0

zephyr_sources_ifdef(CONFIG_OFFLOADER engine.c)
zephyr_sources_ifdef(CONFIG_OFFLOADER_PSA psa.c)

if(CONFIG_OFFLOADER_PSA)

  set(fns
      psa_crypto_init
      psa_import_key
      psa_destroy_key
      psa_generate_random
      psa_hash_compute
      psa_sign_hash
      psa_verify_hash
      psa_aead_encrypt
      psa_aead_decrypt
      psa_mac_compute
      psa_cipher_encrypt
      psa_cipher_decrypt
      psa_generate_key
      psa_generate_key_custom
      psa_sign_message
      psa_verify_message
      psa_asymmetric_encrypt
      psa_asymmetric_decrypt
      psa_raw_key_agreement
      psa_export_public_key
      psa_export_key
      psa_hash_compare
      psa_mac_verify
      psa_key_derivation_key_agreement
      psa_key_derivation_output_key
  )

  foreach(fn ${fns})
    zephyr_link_libraries(-Wl,--wrap=${fn})
  endforeach()

endif()
