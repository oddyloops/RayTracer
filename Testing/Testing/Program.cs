using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testing
{
    class Program
    {
        static Random rand = new Random();

        static void Main(string[] args)
        {
            StringBuilder result = new StringBuilder();
            Console.WriteLine("Running Miracle's");
            result.AppendLine("MIRACLE");
            //Miracle
            int iterations = 6;

            int num = 6;

            
            while (num <= 12)
            {
                for (int i = 0; i < iterations; i++)
                {
                    List<int> l = new List<int>();
                    addRange(l, num);

                    result.AppendLine("Before Sorting (" + i + ":" + num + "----" + printArray(l));
                    long ts = DateTime.Now.Ticks/TimeSpan.TicksPerMillisecond;
                    while (!isSorted(l))
                    {
                        Shuffle(l);
                    }
                    long new_ts = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                    result.AppendLine("Time taken: " + (new_ts - ts) + " ms");
                    result.AppendLine("After Sorting (" + i + ":" + num + "----" + printArray(l));
                    result.AppendLine();
                }
                num += 2;
            }
            result.AppendLine("\n\n");


            Console.WriteLine("Running Kieana's");
            result.AppendLine("KIEANA");
            //Keiana
            iterations = 5;

            num = 5;


            while (num <= 11)
            {
                for (int i = 0; i < iterations; i++)
                {
                    List<int> l = new List<int>();
                    addRange(l, num);

                    result.AppendLine("Before Sorting (" + i + ":" + num + "----" + printArray(l));
                    long ts = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                    while (!isSorted(l))
                    {
                        Shuffle(l);
                    }
                    long new_ts = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                    result.AppendLine("Time taken: " + (new_ts - ts) + " ms");
                    result.AppendLine("After Sorting (" + i + ":" + num + "----" + printArray(l));
                    result.AppendLine();
                }
                num += 2;
            }
            result.AppendLine("\n\n");


            //write to file
            System.IO.File.WriteAllText("results.txt", result.ToString());




        }

        static void addRange(List<int> list, int num)
        {
            for (int i = 0; i < num; i++)
            {
                list.Add(Math.Max(0, rand.Next(200)));
            }
        }

        static bool isSorted(List<int> list)
        {
            bool sorted = true;
            int i = 0;
            while (i < list.Count - 1)
            {
                if (list[i + 1] < list[i])
                {
                    sorted = false;
                    break;
                }
                i++;
            }
            return sorted;
        }

        static string printArray(List<int> list)
        {
            StringBuilder output = new StringBuilder();
            foreach (int i in list)
            {
                output.Append(i + " ");
            }
            return output.ToString();
        }

        static void Shuffle<T>(IList<T> list)
        {
            int n = list.Count;
            while (n > 1)
            {
                n--;
                int k = rand.Next(n + 1);
                T value = list[k];
                list[k] = list[n];
                list[n] = value;
            }
        }
    }

}
