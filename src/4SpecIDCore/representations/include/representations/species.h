#pragma once
#include "record.h"
#include <string>
#include <unordered_set>

namespace ispecid
{
namespace representations
{
using SUSet = std::unordered_set<std::string>;
class Species
{
public:
  Species() {}
  explicit Species(std::string species, std::string grade)
      : m_species(species)
      , m_grade(grade)
      , m_count(0){};

  void setGrade(std::string& grade) { m_grade = grade; };
  void setClusters(SUSet& clusters) { m_clusters = clusters; }
  void setSources(SUSet& sources) { m_sources = sources; }
  void setSize(int size) { m_count = size; }

  std::string getSpeciesName() const { return m_species; }
  const SUSet& getClusters() const { return m_clusters; }
  const SUSet& getSources() const { return m_sources; }
  std::string getGrade() const { return m_grade; }
  int getSourcesSize() const { return getSources().size(); }
  int getClustersSize() const { return getClusters().size(); }
  int size() const { return m_count; }
  std::string getFirstCluster() { return *m_clusters.begin(); }
  bool hasCluster(const std::string& bin) const { return m_clusters.find(bin) == m_clusters.end(); }

  std::string toString()
  {
    std::string out = m_species;
    out = out + " sources:{";
    for (auto& s : m_sources)
    {
      out = out + s + "|";
    }
    out = out + "}";
    out = out + " clusters:{";
    for (auto& s : m_clusters)
    {
      out = out + s + "|";
    }
    out = out + "} ";
    out = out + "grade: " + m_grade;
    out = out + " count: " + std::to_string(m_count) + "\n";
    return out;
  }

  static Species fromRecord(Record& record)
  {
    Species species(record.getSpeciesName(), record.getGrade());
    addRecord(species, record);
    return species;
  }

  static void addRecord(Species& species, Record& record)
  {
    if (species.m_species.empty())
    {
      species.m_species = record.getSpeciesName();
    }
    else if (species.m_species != record.getSpeciesName())
    {
      return;
    }
    species.m_count += record.size();
    species.m_sources.insert(record.getSource());
    species.m_clusters.insert(record.getCluster());
  }

private:
  std::string m_species;
  SUSet m_clusters;
  SUSet m_sources;
  std::string m_grade;
  size_t m_count;
};
} // namespace representations
} // namespace ispecid