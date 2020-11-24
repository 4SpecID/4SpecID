#ifndef SPECIES_H
#define SPECIES_H
#include <string>
#include <unordered_set>
#include "record.h"

namespace ispecid {
namespace datatypes {
class species {
private:
  std::string species_name;
  std::unordered_set<std::string> clusters;
  std::unordered_set<std::string> sources;
  std::string grade;
  int record_count;

public:
  species() {}
  species(std::string _species_name, std::string _grade)
      : species_name(_species_name), grade(_grade), record_count(0){};

  void setGrade(std::string _grade) { this->grade = _grade; };

  std::string getFirstCluster() { return *clusters.begin(); }
  std::string getSpeciesName() { return species_name; }
  void setClusters(std::unordered_set<std::string> cset) { clusters = cset; }
  void setSources(std::unordered_set<std::string> sset) { sources = sset; }
  const std::unordered_set<std::string> &getClusters() { return clusters; }
  const std::unordered_set<std::string> &getSources() { return sources; }
  std::string getGrade() { return grade; }
  void setRecordCount(int count) { record_count = count; }
  int recordCount() { return record_count; }
  int sourcesCount() { return sources.size(); }
  int clustersCount() { return clusters.size(); }

  static species fromRecord(record &record) {
    species species(record.getSpeciesName(), record.getGrade());
    addRecord(species, record);
    return species;
  }

  static void addRecord(species &species, record &record) {
    species.record_count += record.count();
    species.sources.insert(record.getSource());
    species.clusters.insert(record.getCluster());
  }
  std::string toString() {
    std::string out = species_name;
    out = out + " sources:{";
    for (auto &s : sources) {
      out = out + s + "|";
    }
    out = out + "}";
    out = out + " clusters:{";
    for (auto &s : clusters) {
      out = out + s + "|";
    }
    out = out + "} ";
    out = out + "grade: " + grade;
    out = out + " size: " + std::to_string(record_count) + "\n";
    return out;
  }
};
} // namespace datatypes
} // namespace ispecid
#endif