using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.DependencyManagement;
using RTMeld.Services;
using System.Collections.Generic;
using System.Composition;
using System.Composition.Convention;
using System.Composition.Hosting;
using System.IO;
using System.Linq;
using System.Runtime.Loader;

namespace RTContainer
{
    [Export(typeof(ICompositor))]
    public class Compositor : ICompositor
    {
        private CompositionHost container;
        public void Compose(string assemblyDir = "")
        {
            if (string.IsNullOrEmpty(assemblyDir))
            {
                assemblyDir = Directory.GetCurrentDirectory();
            }
            var rule = new ConventionBuilder();
            rule.ForType<IData>().Export<IData>();

            //services and data contexts are singletons
            rule.ForType<IService>().Export<IService>().Shared(); 
            rule.ForType<IDataContext>().Export<IDataContext>().Shared();

            var assemblies = from file in Directory.GetFiles(assemblyDir, "*.dll", SearchOption.AllDirectories)
                             select AssemblyLoadContext.Default.LoadFromAssemblyPath(file);
            var configuration = new ContainerConfiguration().WithAssemblies(assemblies, rule);
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
