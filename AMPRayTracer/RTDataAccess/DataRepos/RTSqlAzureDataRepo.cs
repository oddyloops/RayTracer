using Microsoft.EntityFrameworkCore;
using RTDataAccess.DataObjects;
using RTDataAccess.DataObjects.SqlAzure;
using System;
using System.Collections.Generic;
using System.Text;


namespace RTDataAccess.DataRepos
{
    /// <summary>
    /// An entityframework interface with the SQL Azure interface
    /// </summary>
    public class RTSqlAzureDataRepo : DbContext
    {
        /// <summary>
        /// Constructs an instance of the SQL Azure interface with appropriate connection parameters
        /// </summary>
        /// <param name="options">Encapsulates connection parameters such as connection strings, timeout, etc</param>
        public RTSqlAzureDataRepo(DbContextOptions<RTSqlAzureDataRepo> options) : base(options)
        {
        }

        /// <summary>
        /// A handle for the SQL Azure user table
        /// </summary>
        public DbSet<RTSqlAzureUser> Users { get; set; }

        /// <summary>
        /// A handle for the SQL Azure user session table
        /// </summary>
        public DbSet<RTSqlAzureUserSession> UserSessions { get; set; }
    }
}
