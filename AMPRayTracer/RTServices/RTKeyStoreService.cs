using RTMeld.Config;
using RTMeld.DataAccess;
using RTMeld.Services;
using System;
using System.Collections.Generic;
using System.Composition;
using System.Text;
using Newtonsoft.Json;
using System.IO;
using Newtonsoft.Json.Linq;
using System.Security.Cryptography;
using System.Linq;

namespace RTServices
{
    class KeyData : IEquatable<KeyData>
    {
        public string Index { get; set; }

        public string Key { get; set; }

        public override bool Equals(object obj)
        {
            if(obj != null && obj is KeyData)
            {
                return Index.Equals(((KeyData)obj).Index);
            }
            return false;
        }

        public bool Equals(KeyData other)
        {
            if (other != null)
            {
                return Index.Equals(other.Index);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return Index.GetHashCode();
        }
    }

    [Export(typeof(IKeyStoreService))]
    public class RTKeyStoreService : IKeyStoreService
    {

        [Import("JsonConfig")]
        public IConnectionContext ConfigContext { get; set; }


        private IList<KeyData> keyCache = new List<KeyData>();

        #region HelperMethods
        private void UpdateCache()
        {
            string storageFile = ConfigContext.GetAppSetting(Config.KEY_STORE_PATH);
            if (!File.Exists(storageFile))
            {
                using (File.Create(storageFile))
                {
                    return;
                }
            }
            string json = File.ReadAllText(storageFile);
            JArray keys = JArray.Parse(json);
            keyCache.Clear();
            foreach (var key in keys)
            {
                keyCache.Add(new KeyData() { Index = key["Index"].ToString(), Key = key["Key"].ToString() });
            }
        }

        private void FlushCacheToDisk()
        {
            string storageFile = ConfigContext.GetAppSetting(Config.KEY_STORE_PATH);
            string json = JsonConvert.SerializeObject(keyCache, Formatting.Indented);
            File.WriteAllText(storageFile, json);
        }

        private void CreateAsymKeyIfNotExists()
        {
            string assymKeyContainer = ConfigContext.GetAppSetting(Config.ASYM_KEY_PATH);
            CspParameters csp = new CspParameters()
            {
                KeyContainerName = assymKeyContainer,
                Flags = CspProviderFlags.UseMachineKeyStore
            };
            CspKeyContainerInfo cspKeyContainer = new CspKeyContainerInfo(csp);

            if (!cspKeyContainer.Accessible)
            {
                int keySize = int.Parse(ConfigContext.GetAppSetting(Config.ASYM_KEY_SIZE_BITS));
                using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(keySize, csp))
                {
                    rsa.PersistKeyInCsp = true;
                }
            }
        }

        private byte[] EncryptSymmKey(byte[] symmKey)
        {
            CreateAsymKeyIfNotExists();
            string assymKeyContainer = ConfigContext.GetAppSetting(Config.ASYM_KEY_PATH);
            CspParameters csp = new CspParameters() { KeyContainerName = assymKeyContainer };
            using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(csp))
            {
                return rsa.Encrypt(symmKey, true);
            }

        }

        private byte[] DecryptSymmKey(byte[] encSymmKey)
        {
            CreateAsymKeyIfNotExists();
            string assymKeyContainer = ConfigContext.GetAppSetting(Config.ASYM_KEY_PATH);
            CspParameters csp = new CspParameters() { KeyContainerName = assymKeyContainer };
            using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(csp))
            {
                return rsa.Decrypt(encSymmKey, true);
            }
        }

        private void DeleteAsymmetricKey()
        {
            string assymKeyContainer = ConfigContext.GetAppSetting(Config.ASYM_KEY_PATH);
            CspParameters csp = new CspParameters() { KeyContainerName = assymKeyContainer };
            using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(csp))
            {
                rsa.Clear();
            }
        }
        #endregion


        public void AddOrUpdateKey(string index, byte[] key)
        {
            if (keyCache.Count == 0)
            {
                UpdateCache();
            }

            //encrypt key
            string encKey = Convert.ToBase64String(EncryptSymmKey(key));
            int existingIndex = keyCache.IndexOf(new KeyData() { Index = index });
            if (existingIndex >= 0)
            {
                //exists
                keyCache[existingIndex].Key = encKey;
            }
            else
            {
                keyCache.Add(new KeyData() { Index = index, Key = encKey });
            }
            FlushCacheToDisk();
        }

        public void DeleteKey(string index)
        {
            if (keyCache.Count == 0)
            {
                UpdateCache();
            }

            if (keyCache.Remove(new KeyData() { Key = index }))
            {
                FlushCacheToDisk();
            }
        }

        public byte[] GetKey(string index)
        {
            if(keyCache.Count == 0)
            {
                UpdateCache();
            }

            var result =  from k in keyCache where k.Index == index select k.Key;

            if(result != null && result.Count() > 0)
            {
                return Convert.FromBase64String(result.First());
            }
            return null;
        }

        public void Clear()
        {
            keyCache.Clear();
            string storageFile = ConfigContext.GetAppSetting(Config.KEY_STORE_PATH);
            if (File.Exists(storageFile))
            {
                File.Delete(storageFile);
            }
            DeleteAsymmetricKey();
        }
    }
}
