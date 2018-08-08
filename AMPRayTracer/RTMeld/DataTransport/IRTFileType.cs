
using RTMeld.Attributes;
using System;

namespace RTMeld.DataTransport
{
    [Meta(AzureCosmosDocCollection ="rt_file_type")]
    public interface IRTFileType : ICosmos
    {
   

        [Map("extension,ext")]
        string Ext { get; set; }

        [Map("full_name,Fullname,fullname")]
        string FullName { get; set; }
    }
}
