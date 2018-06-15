using RTDataAccess.DataObjects.SqlAzure;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using Xunit;
using System.Linq;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;

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
        public void TestInsertAndDelete()
        {
            azureContext.Connect();
            azureContext.Insert(testUser);
            var returnedUser = azureContext.SelectOne<IRTUser,Guid>(testUser.Id);
            Assert.Equal(testUser.UserName, returnedUser.UserName);
            azureContext.Delete<Guid,IRTUser>(returnedUser.Id);
            returnedUser = azureContext.SelectOne<IRTUser, Guid>(testUser.Id);
            Assert.Null(returnedUser);
        }


        public void Dispose()
        {
            azureContext.Dispose();
        }
    }
}
