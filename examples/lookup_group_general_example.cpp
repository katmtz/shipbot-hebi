/**
 * An example to lookup a group based on command line arguments.
 *
 * @author Matthew Tesch < matt @ hebirobotics.com >
 * @since 23 Sep 2015
 */ 

#include "lookup_helpers.cpp"

int main(int argc, char* argv[])
{
  // Try and get the requested group
  std::unique_ptr<hebi::Group> group = getGroupFromArgs(argc, argv);
  if (!group)
  {
    printf("Group not found on network.\n\n");
    return -1;
  }

  printf("Found group with %d modules.\n\n", group->size());

  return 0;
}
