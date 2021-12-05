#pragma once
#include <string>

namespace ispecid
{
namespace representations 
{
class Record
{
public:
  explicit Record(std::string species, std::string cluster, std::string source, std::string grade, int count=1)
      : m_species(species)
      , m_cluster(cluster)
      , m_source(source)
      , m_grade(grade)
      , m_count(count){};

  ~Record() {}

  std::string getSpeciesName() const { return m_species; }
  std::string getCluster() const { return m_cluster; }
  std::string getSource() const { return m_source; }
  std::string getGrade() const { return m_grade; }
  int size() const { return m_count; }
  void setSpeciesName(std::string& species) { m_species = species; }
  void setCluster(std::string& cluster) { m_cluster = cluster; }
  void setSource(std::string& source) { m_source = source; }
  void setGrade(std::string& grade) { m_grade = grade; }
  void setSize(int count) { m_count = count; }
  void add(int count) { m_count += count; }

  bool operator==(const Record& other) const
  {
    return m_species == other.m_species && m_cluster == other.m_cluster && m_source == other.m_source;
  }

  std::string toString() const { return m_species + ";" + m_cluster + ";" + m_source + ";" + m_grade; }
  static bool goodRecord(Record& record)
  {
    return record.m_species.empty() || record.m_cluster.empty() || record.m_source.empty();
  }

  static std::string getSpeciesName(Record& record) { return record.getSpeciesName(); }

private:
  std::string m_species;
  std::string m_cluster;
  std::string m_source;
  std::string m_grade;
  int m_count;
};
} // namespace types
} // namespace ispecid