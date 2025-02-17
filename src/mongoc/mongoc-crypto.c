/* Copyright 2016 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mongoc-config.h"

#ifdef MONGOC_ENABLE_CRYPTO

#include <bson.h>
#include "mongoc-log.h"
#include "mongoc-crypto-private.h"
#ifdef MONGOC_ENABLE_LIBCRYPTO
#include "mongoc-crypto-openssl-private.h"
#elif defined(MONGOC_ENABLE_COMMON_CRYPTO)
#include "mongoc-crypto-common-crypto-private.h"
#endif

void
mongoc_crypto_init (mongoc_crypto_t *crypto)
{
   switch(MONGOC_CRYPTO_TYPE)
   {
#ifdef MONGOC_ENABLE_COMMON_CRYPTO
      case MONGOC_CRYPTO_COMMON_CRYPTO:
         crypto->hmac_sha1 = mongoc_crypto_common_crypto_hmac_sha1;
         crypto->sha1 = mongoc_crypto_common_crypto_sha1;
         break;
#endif
#ifdef MONGOC_ENABLE_LIBCRYPTO
      case MONGOC_CRYPTO_OPENSSL:
         crypto->hmac_sha1 = mongoc_crypto_openssl_hmac_sha1;
         crypto->sha1 = mongoc_crypto_openssl_sha1;
         break;
#endif
      default:
         MONGOC_ERROR("Unknown crypto engine");
   }
}

void
mongoc_crypto_hmac_sha1 (mongoc_crypto_t     *crypto,
                         const void          *key,
                         int                  key_len,
                         const unsigned char *d,
                         int                  n,
                         unsigned char       *md /* OUT */)
{
   crypto->hmac_sha1 (crypto, key, key_len, d, n, md);
}

bool
mongoc_crypto_sha1      (mongoc_crypto_t     *crypto,
                         const unsigned char *input,
                         const size_t         input_len,
                         unsigned char       *output /* OUT */)
{
   return crypto->sha1 (crypto, input, input_len, output);
}
#endif

