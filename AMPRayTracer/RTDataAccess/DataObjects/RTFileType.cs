using RTMeld.DataTransport;

namespace RTDataAccess.DataObjects
{
    public class RTFileType : IRTFileType
    {
        protected int id;

        protected string ext;

        protected string fullName;

        public int Id { get => id; set => id = value; }
        public string Ext { get => ext; set => ext = value; }
        public string FullName { get => fullName; set => fullName = value; }
    }
}
