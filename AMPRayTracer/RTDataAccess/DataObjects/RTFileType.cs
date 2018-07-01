using RTMeld.DataTransport;

namespace RTDataAccess.DataObjects
{
    public class RTFileType : IRTFileType
    {
        private int id;

        private string ext;

        private string fullName;

        public int Id { get => id; set => id = value; }
        public string Ext { get => ext; set => ext = value; }
        public string FullName { get => fullName; set => fullName = value; }

        public override bool Equals(object obj)
        {
            if(obj != null && obj is RTFileType)
            {
                var temp = (RTFileType)obj;
                return Id.Equals(temp.Id);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return id.GetHashCode();
        }
    }
}
