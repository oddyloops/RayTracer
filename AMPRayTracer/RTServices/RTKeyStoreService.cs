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

namespace RTServices
{
    class KeyData
    {
        public string Index { get; set; }

        public string Key { get; set; }
    }
    public class RTKeyStoreService : IKeyStoreService
    {

        [Import("JsonConfig")]
        public IConnectionContext ConfigContext { get; set; }


        private string storageFile = "keyStore.json";

        private IList<KeyData> keyCache = new List<KeyData>();

        #region HelperMethods
        private void UpdateCache()
        {
            string json = File.ReadAllText(storageFile);
            JArray keys = JArray.Parse(json);
            keyCache.Clear();
            foreach(var key in keys)
            {
                keyCache.Add(new KeyData() { Index = key["Index"].ToString(), Key = key["Key"].ToString() });
            }
        }

        private void FlushCacheToDisk()
        {
            string json = JsonConvert.SerializeObject(keyCache, Formatting.Indented);
            File.WriteAllText(storageFile, json);
        }
        #endregion


        public RTKeyStoreService()
        {
            string configStorageFile = ConfigContext.GetAppSetting(Config.KEY_STORE_PATH);
            if(configStorageFile != null)
            {
                storageFile = configStorageFile;
            }
        }
        public void AddOrUpdateKey(string index, byte[] key)
        {
            if(keyCache.Count == 0)
            {
                keyCache = 
            }
        }

        public void DeleteKey(string index)
        {
            throw new NotImplementedException();
        }

        public byte[] GetKey(string index)
        {
            
        }

        
    }
}
