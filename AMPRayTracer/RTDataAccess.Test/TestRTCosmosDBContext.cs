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
                Id =1,
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
        public void TestInsertAndDelete()
        {
            cosmosContext.Connect();
            cosmosContext.Insert(testFileType);
            var returned = cosmosContext.SelectOne<RTCosmosFileType, int>(testFileType.Id);
            Assert.Equal(testFileType.FullName, returned.FullName);
            cosmosContext.Delete<int, RTCosmosFileType>(returned.Id);
            returned = cosmosContext.SelectOne<RTCosmosFileType, int>(testFileType.Id);
            Assert.Null(returned);
        }
    }
}
