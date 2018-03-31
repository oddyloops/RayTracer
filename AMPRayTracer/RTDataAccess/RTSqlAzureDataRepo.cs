using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Text;


namespace RTDataAccess.DataObjects
{
    public class RTSqlAzureDataRepo : DbContext
    {
        public RTSqlAzureDataRepo(DbContextOptions<RTSqlAzureDataRepo> options) : base(options)
        {
        }


    }
}
