using RTMeld.DataAccess;
using System;
using Xunit;

namespace RTDataAccess.Test
{
    public class TestRTSqlAzureContext
    {
        IDataContext azureContext;

        public TestRTSqlAzureContext()
        {
            azureContext = new RTSqlAzureContext();
           
        }
        
        [Fact]
        public void TestConnect()
        {
            azureContext.Connect();
        }
    }
}
