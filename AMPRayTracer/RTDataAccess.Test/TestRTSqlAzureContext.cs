using RTDataAccess.DataObjects.SqlAzure;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using Xunit;
using System.Linq;
using System;

namespace RTDataAccess.Test
{
    public class TestRTSqlAzureContext
    {
        IDataContext azureContext;
        IDataMapper mapper;
        IConnectionContext context;
        IRTUser testUser;

        public TestRTSqlAzureContext()
        {
            mapper = new RTMapper.RTMapper();
            context = new TestConnectionContext();
            azureContext = new RTSqlAzureContext(context,mapper);
            testUser = new RTSqlAzureUser()
            { 
                 Id = Guid.NewGuid(),
                 Email = "rt@raytrace.com",
                 Password = System.Text.Encoding.Unicode.GetBytes("1234"),
                 UserName="rt"

            };
        }
        
        [Fact]
        public void TestConnect()
        {
            azureContext.Connect();
        }

        [Fact]
        public void TestInsert()
        {
            azureContext.Insert(testUser);
            var results= azureContext.SelectAll<IRTUser>().ToList();
            Assert.True(results.Count > 0);
            var returnedUser = results.First();
            Assert.Equal(testUser.UserName, returnedUser.UserName);
            azureContext.Delete<Guid,IRTUser>(returnedUser.Id);
        }


        public void Dispose()
        {
            azureContext.Dispose();
        }
    }
}
