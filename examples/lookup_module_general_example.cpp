/**
 * An example to lookup a module based on command line arguments.
 *
 * @author Matthew Tesch < matt @ hebirobotics.com >
 * @since 23 Sep 2015
 */

#include "lookup_helpers.cpp"

int main(int argc, char* argv[])
{
  // Try and get the requested module.
  std::unique_ptr<hebi::Module> module = getModuleFromArgs(argc, argv);
  if (!module)
  {
    printf("Module not found on network.\n\n");
    return -1;
  }

  printf("Found module.\n\n");

  return 0;
}
