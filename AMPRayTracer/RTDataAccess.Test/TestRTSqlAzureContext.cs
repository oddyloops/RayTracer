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
        IList<RTSqlAzureUser> testUserList = new List<RTSqlAzureUser>()
        {
            new RTSqlAzureUser(){ Id = Guid.NewGuid(), Email ="user1@me.com", Password = System.Text.Encoding.Unicode.GetBytes("1234"), UserName ="user1"},
                        new RTSqlAzureUser(){ Id = Guid.NewGuid(), Email ="user2@me.com", Password = System.Text.Encoding.Unicode.GetBytes("1234"), UserName ="user2"}

        };

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
            var returnedUser = azureContext.SelectOne<RTSqlAzureUser>(testUser.Id);
            Assert.Equal(testUser.UserName, returnedUser.UserName);
            azureContext.Delete<RTSqlAzureUser>(returnedUser.Id);
            returnedUser = azureContext.SelectOne<RTSqlAzureUser>(testUser.Id);
            Assert.Null(returnedUser);
        }


        [Fact]
        public async void TestInsertAndDeleteAsync()
        {
            azureContext.Connect();
            await azureContext.InsertAsync(testUser);
            var returnedUser = await azureContext.SelectOneAsync<RTSqlAzureUser>(testUser.Id);
            Assert.Equal(testUser.UserName, returnedUser.UserName);
            await azureContext.DeleteAsync<RTSqlAzureUser>(returnedUser.Id);
            returnedUser = await azureContext.SelectOneAsync<RTSqlAzureUser>(testUser.Id);
            Assert.Null(returnedUser);
        }

        [Fact]
        public void TestSelectAndDeleteMatching()
        {
            azureContext.Connect();
            azureContext.Insert(testUser);
            var returned = azureContext.SelectMatching<RTSqlAzureUser>(
                    x => x.UserName.Equals(testUser.UserName)
                    );
            Assert.NotNull(returned);
            Assert.NotEmpty(returned);
            azureContext.DeleteMatching<RTSqlAzureUser>(
                x => x.UserName.Equals(testUser.UserName));
            returned = azureContext.SelectMatching<RTSqlAzureUser>(
                    x => x.UserName.Equals(testUser.UserName)
                    );
            Assert.Empty(returned);

        }

        [Fact]
        public async void TestSelectAndDeleteMatchingAsync()
        {
            azureContext.Connect();
            await azureContext.InsertAsync(testUser);
            var returned = await azureContext.SelectMatchingAsync<RTSqlAzureUser>(
                    x => x.UserName.Equals(testUser.UserName)
                    );
            Assert.NotNull(returned);
            Assert.NotEmpty(returned);
            await azureContext.DeleteMatchingAsync<RTSqlAzureUser>(
                x => x.UserName.Equals(testUser.UserName));
            returned = await azureContext.SelectMatchingAsync<RTSqlAzureUser>(
                    x => x.UserName.Equals(testUser.UserName)
                    );
            Assert.Empty(returned);

        }


        [Fact]
        public void TestInsertSelectDeleteQueries()
        {
            string insert = "INSERT INTO RT_User(UserId,Username,Pwd,Email) VALUES(@P0,@P1,@P2,@P3)";
            azureContext.Connect();
            IDictionary<string, object> parameters = new Dictionary<string, object>();
            Guid id = Guid.NewGuid();
            parameters.Add("@P0", id);
            parameters.Add("@P1", testUser.UserName);
            parameters.Add("@P2", testUser.Password);
            parameters.Add("@P3", testUser.Email);
            azureContext.ExecuteNonQuery(insert, parameters);
            string select = "SELECT * FROM RT_User WHERE UserId=@P0";
            parameters.Clear();
            parameters.Add("@P0", id);
            var result = azureContext.Query<RTSqlAzureUser>(select, parameters);
            Assert.NotNull(result);
            Assert.NotEmpty(result.ToList());
            string delete = "DELETE FROM RT_User WHERE UserId=@P0";
            azureContext.ExecuteNonQuery(delete, parameters);
            result = azureContext.Query<RTSqlAzureUser>(select, parameters);
            Assert.NotNull(result);
            Assert.Empty(result.ToList());
        }


        [Fact]
        public void TestUpdate()
        {
            azureContext.Connect();
            azureContext.Insert(testUser);
            string oldUsername = testUser.UserName;
            string newUsername = "Modified";
            testUser.UserName = newUsername;
            azureContext.Update(testUser.Id, (RTSqlAzureUser)testUser, true);
            var user = azureContext.SelectOne<RTSqlAzureUser>(testUser.Id);
            Assert.NotNull(user);
            Assert.Equal(newUsername, user.UserName);
            testUser.UserName = oldUsername;
            azureContext.Update(testUser.Id, (RTSqlAzureUser)testUser, true);
            user = azureContext.SelectOne<RTSqlAzureUser>(testUser.Id);
            Assert.NotNull(user);
            Assert.Equal(oldUsername, user.UserName);
            azureContext.Delete<RTSqlAzureUser>(testUser.Id);
          

        }


        [Fact]
        public async void TestUpdateAsync()
        {
            azureContext.Connect();
            azureContext.Insert(testUser);
            string oldUsername = testUser.UserName;
            string newUsername = "Modified";
            testUser.UserName = newUsername;
            await azureContext.UpdateAsync(testUser.Id, (RTSqlAzureUser)testUser, true);
            var user = azureContext.SelectOne<RTSqlAzureUser>(testUser.Id);
            Assert.NotNull(user);
            Assert.Equal(newUsername, user.UserName);
            testUser.UserName = oldUsername;
            await azureContext.UpdateAsync(testUser.Id, (RTSqlAzureUser)testUser, true);
            user = azureContext.SelectOne<RTSqlAzureUser>(testUser.Id);
            Assert.NotNull(user);
            Assert.Equal(oldUsername, user.UserName);
            await azureContext.DeleteAsync<RTSqlAzureUser>(testUser.Id);

        }

        [Fact]
        public void TestInsertAndDeleteAll()
        {
            azureContext.Connect();
            azureContext.InsertAll(testUserList);
            var resultList = azureContext.SelectMatching<RTSqlAzureUser>(x => x.UserName.StartsWith("user"));
            Assert.NotNull(resultList);
            Assert.NotEmpty(resultList);
            Assert.Equal(testUserList.Count, resultList.Count());
            azureContext.DeleteAll(testUserList);
            resultList = azureContext.SelectMatching<RTSqlAzureUser>(x => x.UserName.StartsWith("user"));
            Assert.NotNull(resultList);
            Assert.Empty(resultList);
        }


        [Fact]
        public async void TestInsertAndDeleteAllAsync()
        {
            azureContext.Connect();
            await azureContext.InsertAllAsync(testUserList);
            var resultList = azureContext.SelectMatching<RTSqlAzureUser>(x => x.UserName.StartsWith("user"));
            Assert.NotNull(resultList);
            Assert.NotEmpty(resultList);
            Assert.Equal(testUserList.Count, resultList.Count());
            await azureContext.DeleteAllAsync(testUserList);
            resultList = azureContext.SelectMatching<RTSqlAzureUser>(x => x.UserName.StartsWith("user"));
            Assert.NotNull(resultList);
            Assert.Empty(resultList);
        }


        [Fact]
        public void TestUpdateAll()
        {
            azureContext.Connect();
            azureContext.InsertAll(testUserList);
            string newEmail = "free2rhyme@me.com";
            IRTUser newData = new RTSqlAzureUser()
            {
                Email = newEmail
            };
            azureContext.UpdateAll(testUserList, (RTSqlAzureUser)newData, true);
            var resultList = azureContext.SelectMatching<RTSqlAzureUser>(x => x.Email.Equals(newEmail));
            Assert.NotNull(resultList);
            Assert.NotEmpty(resultList);
            Assert.Equal(testUserList.Count, resultList.Count());
            azureContext.DeleteAll(testUserList);

        }


        [Fact]
        public async void TestUpdateAllAsync()
        {
            azureContext.Connect();
            azureContext.InsertAll(testUserList);
            string newEmail = "free2rhyme@me.com";
            IRTUser newData = new RTSqlAzureUser()
            {
                Email = newEmail
            };
            await azureContext.UpdateAllAsync(testUserList, (RTSqlAzureUser)newData, true);
            var resultList = azureContext.SelectMatching<RTSqlAzureUser>(x => x.Email.Equals(newEmail));
            Assert.NotNull(resultList);
            Assert.NotEmpty(resultList);
            Assert.Equal(testUserList.Count, resultList.Count());
            azureContext.DeleteAll(testUserList);

        }

        public void Dispose()
        {
            azureContext.Dispose();
        }
    }
}
