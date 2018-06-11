using RTMeld.DataAccess;
using System;
using Xunit;

namespace RTDataAccess.Test
{
    public class TestRTSqlAzureContext
    {
        IDataContext azureContext;
        IDataMapper mapper;
        IConnectionContext context;

        public TestRTSqlAzureContext()
        {
            mapper = new RTMapper();
            azureContext = new RTSqlAzureContext();
           
        }
        
        [Fact]
        public void TestConnect()
        {
            azureContext.Connect();
        }
    }
}
