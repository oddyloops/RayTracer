using RTDataAccess.DataObjects.Cosmos;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace RTDataAccess.Test
{
    public class TestRTCosmosDBContext
    {
        IDataContext cosmosContext;
        IDataMapper mapper;
        IConnectionContext context;
        IRTFileType testFileType;
        IList<RTCosmosFileType> testFileTypeList = new List<RTCosmosFileType>() {
            new RTCosmosFileType() { Id = Guid.NewGuid().ToString(), Ext ="jpg", FullName ="Joint Photography Experts Group"},
            new RTCosmosFileType() { Id = Guid.NewGuid().ToString(), Ext ="png", FullName="Portable Network Graphic"}
        };


        public TestRTCosmosDBContext()
        {
            mapper = new RTMapper.RTMapper();
            context = new TestConnectionContext();
            cosmosContext = new RTCosmoDBContext(context, mapper);

            testFileType = new RTCosmosFileType()
            {
                Id = Guid.NewGuid().ToString(),
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
        public async void TestInsertAndDeleteAsync()
        {
            cosmosContext.Connect();
            await cosmosContext.InsertAsync(testFileType);
            var returned = await cosmosContext.SelectOneAsync<RTCosmosFileType>(testFileType.Id);
            Assert.Equal(testFileType.FullName, returned.FullName);
            await cosmosContext.DeleteAsync<RTCosmosFileType>(returned.Id);
            returned = await cosmosContext.SelectOneAsync<RTCosmosFileType>(testFileType.Id);
            Assert.Null(returned);
        }


        [Fact]
        public async void TestQuery()
        {
            string select = "SELECT * FROM rt_file_type F WHERE F.id =@P0";
            IDictionary<string, object> parameters = new Dictionary<string, object>();
            parameters.Add("@P0", testFileType.Id);
            cosmosContext.Connect();
            await cosmosContext.InsertAsync(testFileType);
            var returned = cosmosContext.Query<RTCosmosFileType>(select,parameters).ToList();
            Assert.NotEmpty(returned);
            Assert.Equal(testFileType.FullName, returned[0].FullName);
            await cosmosContext.DeleteAsync<RTCosmosFileType>(testFileType.Id);
            returned = cosmosContext.Query<RTCosmosFileType>(select, parameters).ToList();
            Assert.Empty(returned);
        }


        [Fact]
        public async void TestQueryAsync()
        {
            string select = "SELECT * FROM rt_file_type F WHERE F.id =@P0";
            IDictionary<string, object> parameters = new Dictionary<string, object>();
            parameters.Add("@P0", testFileType.Id);
            cosmosContext.Connect();
            await cosmosContext.InsertAsync(testFileType);
            var returned =await cosmosContext.QueryAsync<RTCosmosFileType>(select, parameters);
            Assert.NotEmpty(returned);
            Assert.Equal(testFileType.FullName, returned[0].FullName);
            await cosmosContext.DeleteAsync<RTCosmosFileType>(testFileType.Id);
            returned = await cosmosContext.QueryAsync<RTCosmosFileType>(select, parameters);
            Assert.Empty(returned);
        }


        [Fact]
        public async void TestUpdateAsync()
        {
            string newName = "Jpeg";
            cosmosContext.Connect();
            await cosmosContext.InsertAsync(testFileType);
            string oldName = testFileType.FullName;
            testFileType.FullName = newName;
            await cosmosContext.UpdateAsync(testFileType.Id, (RTCosmosFileType)testFileType, true);
            var file = await cosmosContext.SelectOneAsync<RTCosmosFileType>(testFileType.Id);
            Assert.NotNull(file);
            Assert.Equal(newName, file.FullName);
            testFileType.FullName = oldName;
            await cosmosContext.UpdateAsync(testFileType.Id, (RTCosmosFileType)testFileType, true);
            file = await cosmosContext.SelectOneAsync<RTCosmosFileType>(testFileType.Id);
            Assert.NotNull(file);
            Assert.Equal(oldName, file.FullName);
            await cosmosContext.DeleteAsync<RTCosmosFileType>(testFileType.Id);
        }

        
    }
}
