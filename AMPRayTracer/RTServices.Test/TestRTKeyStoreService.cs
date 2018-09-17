using RTMeld;
using RTMeld.Services;
using System;
using Xunit;

namespace RTServices.Test
{
    public class TestRTKeyStoreService
    {

        IKeyStoreService service;

        const string KEY_INDEX = "key1";

        byte[] key;

        public TestRTKeyStoreService()
        {
            service = new RTKeyStoreService();
            service.ConfigContext = new TestConnectionContext();
            key = new byte[] { 1, 2, 3, 4 };
        }


        [Fact]
        public void TestAddAndDeleteKey()
        {
            service.AddOrUpdateKey(KEY_INDEX, key);
            Assert.True(Util.ArrayEquals(key, service.GetKey(KEY_INDEX)));
            service.DeleteKey(KEY_INDEX);
            Assert.Null(service.GetKey(KEY_INDEX));
            service.Clear();
        }

        [Fact]
        public void TestUpdateKey()
        {
            service.AddOrUpdateKey(KEY_INDEX, key);
            key = new byte[] { 4, 5, 6, 7 };
            service.AddOrUpdateKey(KEY_INDEX, key);
            Assert.True(Util.ArrayEquals(key, service.GetKey(KEY_INDEX)));
            service.DeleteKey(KEY_INDEX);
            service.Clear();
        }

        [Fact]
        public void TestClear()
        {
            service.AddOrUpdateKey(KEY_INDEX, key);
            service.Clear();
            Assert.Null(service.GetKey(KEY_INDEX));
            service.Clear();
        }
    }
}
