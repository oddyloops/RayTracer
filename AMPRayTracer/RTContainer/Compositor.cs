using RTMeld.DataTransport;
using RTMeld.DependencyManagement;
using System.Collections.Generic;
using System.Composition.Hosting;
using System.IO;
using System.Linq;
using System.Runtime.Loader;

namespace RTContainer
{
    public class Compositor : ICompositor
    {
        private CompositionHost container;
        public void Compose(string assemblyDir = "")
        {
            if (string.IsNullOrEmpty(assemblyDir))
            {
                assemblyDir = Directory.GetCurrentDirectory();
            }
            var assemblies = from file in Directory.GetFiles(assemblyDir, "*.dll", SearchOption.AllDirectories)
                             select AssemblyLoadContext.Default.LoadFromAssemblyPath(file);
            var configuration = new ContainerConfiguration().WithAssemblies(assemblies);
            container = configuration.CreateContainer();

            
        }

        public T GetInstance<T>()
        {
            return container.GetExport<T>();
        }


        public IEnumerable<T> GetInstances<T>()
        {
            return container.GetExports<T>();
        }


    }
}
