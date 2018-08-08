using RTDataAccess.DataObjects.Cosmos;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.Text;
using Xunit;

namespace RTDataAccess.Test
{
    public class TestRTCosmosDBContext
    {
        IDataContext cosmosContext;
        IDataMapper mapper;
        IConnectionContext context;
        IRTFileType testFileType;

        public TestRTCosmosDBContext()
        {
            mapper = new RTMapper.RTMapper();
            context = new TestConnectionContext();
            cosmosContext = new RTCosmoDBContext(context, mapper);

            testFileType = new RTCosmosFileType()
            {
                FullName = "Bitmap",
                Ext = "bmp"
            };
        }


        [Fact]
        public void TestConnect()
        {
            cosmosContext.Connect();
        }


        [Fact]
        public async void TestInsertAndDelete()
        {
            cosmosContext.Connect();
            await cosmosContext.InsertAsync(testFileType);
            var returned = await cosmosContext.SelectOneAsync<RTCosmosFileType, string>(testFileType.Id);
            Assert.Equal(testFileType.FullName, returned.FullName);
            await cosmosContext.DeleteAsync<string, RTCosmosFileType>(returned.Id);
            returned = await cosmosContext.SelectOneAsync<RTCosmosFileType, string>(testFileType.Id);
            Assert.Null(returned);
        }
    }
}
