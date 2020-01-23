/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef RMF_TRAFFIC__SCHEDULE__QUERY_HPP
#define RMF_TRAFFIC__SCHEDULE__QUERY_HPP

#include <rmf_traffic/detail/bidirectional_iterator.hpp>

#include <rmf_traffic/schedule/Version.hpp>
#include <rmf_traffic/schedule/Participant.hpp>

#include <rmf_traffic/Region.hpp>
#include <rmf_traffic/Time.hpp>

#include <rmf_utils/impl_ptr.hpp>

#include <unordered_set>
#include <vector>

namespace rmf_traffic {
namespace schedule {

//==============================================================================
/// A class to define a query into a schedule database.
class Query
{
public:

  template<typename E, typename I, typename F>
  using base_iterator = rmf_traffic::detail::bidirectional_iterator<E, I, F>;

  /// A class to describe spacetime filters for a schedule Query.
  class Spacetime
  {
  public:

    using Space = geometry::Space;

    /// This enumerator determines what Spacetime mode the query will be in.
    enum class Mode : uint16_t
    {
      /// Invalid mode, behavior is undefined.
      Invalid,

      /// Request trajectories throughout all of space and time. This will still
      /// be constrained by the version field.
      All,

      /// Request trajectories in specific regions spacetime regions.
      Regions,

      /// Request trajectories that are active in a specified timespan.
      Timespan,
    };

    //==========================================================================
    /// This is a placeholder class in case we ever want to extend the features
    /// of the `All` mode.
    class All
    {
    public:

      class Implementation;
    private:
      rmf_utils::impl_ptr<Implementation> _pimpl;
    };

    //==========================================================================
    /// A container class for rmf_traffic::Region instances. Using Regions mode
    /// will query for Trajectories that intersect the specified regions.
    class Regions
    {
    public:

      class IterImpl;
      using iterator = base_iterator<Region, IterImpl, Regions>;
      using const_iterator = base_iterator<const Region, IterImpl, Regions>;

      /// Add a Region to this container.
      void push_back(Region region);

      /// Remove the last Region that was added to this container.
      void pop_back();

      /// Erase a Region based on its iterator.
      iterator erase(iterator it);

      /// Erase a range of Regions based on their iterators.
      iterator erase(iterator first, iterator last);

      /// Get the beginning iterator of this container.
      iterator begin();

      /// const-qualified begin()
      const_iterator begin() const;

      /// Explicitly const-qualified alternative to begin()
      const_iterator cbegin() const;

      /// Get the one-past-the-end iterator of this container.
      iterator end();

      /// const-qualified end()
      const_iterator end() const;

      /// Explicitly const-qualified alternative to end()
      const_iterator cend() const;

      /// Get the number of Spacetime Region elements in this container.
      std::size_t size() const;

      class Implementation;
    private:
      Regions();
      rmf_utils::impl_ptr<Implementation> _pimpl;
    };

    //==========================================================================
    /// A class for specifying a timespan.
    class Timespan
    {
    public:

      /// Get the maps that will be queried.
      const std::unordered_set<std::string>& get_maps() const;

      /// Add a map to the query.
      Timespan& add_map(std::string map_name);

      /// Remove a map from the query.
      Timespan& remove_map(const std::string& map_name);

      /// Get the lower bound for the time range.
      ///
      /// If there is no lower bound for the time range, then this returns a
      /// nullptr.
      const Time* get_lower_time_bound() const;

      /// Set the lower bound fore the time range.
      Timespan& set_lower_time_bound(Time time);

      /// Remove the lower bound for the time range.
      Timespan& remove_lower_time_bound();

      /// Get the upper bound for the time range.
      ///
      /// If there is no upper bound for the time range, then this returns
      /// a nullptr.
      const Time* get_upper_time_bound() const;

      /// Set the upper bound for the time range.
      Timespan& set_upper_time_bound(Time time);

      /// Remove the upper bound for the time range.
      Timespan& remove_upper_time_bound();

      class Implementation;
    private:
      Timespan();
      rmf_utils::impl_ptr<Implementation> _pimpl;
    };

    /// Default constructor, uses All mode.
    Spacetime();

    /// Regions mode constructor.
    ///
    /// \param[in] regions
    ///   The regions to use
    Spacetime(std::vector<Region> regions);

    /// Timespan mode constructor.
    ///
    /// This will query all trajectories across all time for the specified maps.
    ///
    /// \param[in] maps
    ///   The maps to query from
    Spacetime(std::vector<std::string> maps);

    /// Timespan mode constructor.
    ///
    /// This will query all trajectories that have at least one waypoint active
    /// after the lower bound on the specified maps.
    ///
    /// \param[in] maps
    ///   The maps to query from
    ///
    /// \param[in] lower_bound
    ///   The lower bound on time
    Spacetime(
        std::vector<std::string> maps,
        Time lower_bound);

    /// Timespan mode constructor.
    ///
    /// This will query all trajectories that have at least one waypoint active
    /// after the lower bound and before the upper bound on the specified maps.
    ///
    /// \param[in] maps
    ///   The maps to query from
    ///
    /// \param[in] lower_bound
    ///   The lower bound on time
    ///
    /// \param[in] upper_bound
    ///   The upper bound on time
    Spacetime(
        std::vector<std::string> maps,
        Time lower_bound,
        Time upper_bound);

    /// Get the current Spacetime Mode of this query.
    Mode get_mode() const;

    /// Set the mode of this Spacetime to query for All Trajectories throughout
    /// Spacetime.
    All& query_all();

    /// Set the mode of this Spacetime to query for specific Regions.
    ///
    /// \param[in] regions
    ///   Specify the regions of Spacetime to use.
    Regions& query_regions(std::vector<Region> regions = {});

    /// Get the Regions of Spacetime to use for this Query. If this Spacetime is
    /// not in Regions mode, then this will return a nullptr.
    Regions* regions();

    /// const-qualified regions()
    const Regions* regions() const;

    /// Query a timespan between two bounds for a set of maps
    Timespan& query_timespan(
        std::vector<std::string> maps,
        Time lower_bound,
        Time upper_bound);

    /// Query from a lower bound in time for a set of maps
    Timespan& query_timespan(
        std::vector<std::string> maps,
        Time lower_bound);

    /// Query for all trajectories on a set of maps
    Timespan& query_timespan(std::vector<std::string> maps);

    /// Get the Timespan of Spacetime to use for this Query. If this Spacetime
    /// is not in Timespan mode, then this will return a nullptr.
    Timespan* timespan();

    /// const-qualified timespan()
    const Timespan* timespan() const;

    class Implementation;
  private:
    rmf_utils::impl_ptr<Implementation> _pimpl;
  };

  /// A class to describe a filter on what version changes to query from a
  /// schedule.
  class Versions
  {
  public:

    /// The mode for how to filter versions in a schedule database query.
    enum class Mode : uint16_t
    {
      /// Invalid mode, behavior is undefined.
      Invalid,

      /// Get everything, regardless of version.
      All,

      /// Get every version after the specified one.
      After,
    };

    /// This is a placeholder class in case we ever want to extend the features
    /// of the `All` mode.
    class All
    {
    public:

      class Implementation;
    private:
      All();
      friend class Participants;
      rmf_utils::impl_ptr<Implementation> _pimpl;
    };

    /// The interface for the Versions::After mode.
    class After
    {
    public:

      /// Constructor.
      After(Version version);

      /// Get the specified version. The Query will only return Trajectories
      /// which were introduced after this version of the schedule.
      Version get_version() const;

      /// Set the version.
      ///
      /// \param[in] version
      ///   The Query will only return Trajectories which were introduced after
      ///   this version of the schedule.
      After& set_version(Version version);

      class Implementation;
    private:
      /// Default constructor. This is not accessible to users because it leaves
      /// the After instance null.
      After();
      friend class Versions;
      rmf_utils::impl_ptr<Implementation> _pimpl;
    };

    /// Default constructor, uses All mode.
    Versions();

    /// Constructor to use After mode.
    ///
    /// \param[in] version
    ///   The Query will only return Trajectories which were introduced after
    ///   this version of the schedule.
    Versions(Version version);

    /// Get the current Versions mode of this query.
    Mode get_mode() const;

    /// Set the mode of this Versions interface to query for All Trajectories
    /// regardless of version.
    All& query_all();

    /// Set the mode of this Versions interface to query for only Trajectories
    /// that changed after the given version.
    ///
    /// \param[in] version
    ///   The Query will only return Trajectories which were introduced after
    ///   this version of the schedule.
    After& query_after(Version version);

    /// Get the Versions After interface to use for this Query. If this Versions
    /// is not in the After mode, then this will return a nullptr.
    After* after();

    /// const-qualified after()
    const After* after() const;

    class Implementation;
  private:
    rmf_utils::impl_ptr<Implementation> _pimpl;
  };

  /// A class to describe a filter on which schedule participants to pay
  /// attention to.
  class Participants
  {
  public:

    enum class Mode : uint16_t
    {
      /// Invalid mode, behavior is undefined
      Invalid,

      /// Get all participants
      All,

      /// Get only the participants listed
      Include,

      /// Get all participants except the ones listed
      Exclude
    };

    /// This is a placeholder class in case we ever want to extend the features
    /// of the `All` mode.
    class All
    {
    public:

      class Implementation;
    private:
      All();
      friend class Participants;
      rmf_utils::impl_ptr<Implementation> _pimpl;
    };

    /// The interface for the Participants::Include mode
    class Include
    {
    public:

      /// Constructor.
      Include(std::vector<ParticipantId> ids);

      /// Get the IDs of the participants that should be included.
      const std::vector<ParticipantId>& get_ids() const;

      /// Set the IDs of the participants that should be included.
      Include& set_ids(std::vector<ParticipantId> ids);

      class Implementation;
    private:
      Include();
      friend class Participants;
      rmf_utils::impl_ptr<Implementation> _pimpl;
    };

    /// The interface for the Participants::Exclude mode
    class Exclude
    {
    public:

      /// Constructor
      Exclude(std::vector<ParticipantId> ids);

      /// Get the IDs of the participants that should be excluded.
      const std::vector<ParticipantId>& get_ids() const;

      /// Set the IDs of the participants that should be excluded.
      Exclude& set_ids(std::vector<ParticipantId> ids);

      class Implementation;
    private:
      Exclude();
      friend class Participants;
      rmf_utils::impl_ptr<Implementation> _pimpl;
    };

    /// Default constructor, uses All mode.
    Participants();

    /// Constructor to use All mode.
    static Participants make_all();

    /// Constructor to use Include mode.
    ///
    /// \param[in] ids
    ///   The IDs of the participants that should be included in the query.
    static Participants make_only(std::vector<ParticipantId> ids);

    /// Constructor to use Exclude mode.
    ///
    /// \param[in] ids
    ///   The IDs of the participants that should be excluded from the query.
    static Participants make_all_except(std::vector<ParticipantId> ids);

    /// Get the All interface if this Participants filter is in All mode,
    /// otherwise get a nullptr.
    All* all();

    /// const-qualified all()
    const All* all() const;

    /// Get the Include interface if this Participants filter is in Include
    /// mode, otherwise get a nullptr.
    Include* include();

    /// const-qualified include()
    const Include* include() const;

    /// Get the Exclude interface if this Participants filter is in Exclude
    /// mode, otherwise get a nullptr.
    Exclude* exclude();

    /// const-qualified exclude()
    const Exclude* exclude() const;

  private:
    class Implementation;
    rmf_utils::impl_ptr<Implementation> _pimpl;
  };

  /// Get the Spacetime component of this Query.
  Spacetime& spacetime();

  /// const-qualified spacetime()
  const Spacetime& spacetime() const;

  /// Get the Versions component of this Query.
  Versions& versions();

  /// const-qualified versions()
  const Versions& versions() const;

  class Implementation;
private:
  /// \internal The default constructor is private because users are expected
  /// to construct a Query using one of the functions below.
  Query();
  rmf_utils::impl_ptr<Implementation> _pimpl;
};

//==============================================================================
/// Query for all Trajectories in a schedule database
Query query_everything();

//==============================================================================
/// Query for all Trajectories in a schedule database that were introduced
/// after a specified version of the schedule.
///
/// \param[in] after_version
///   Only query Trajectories that were added to the schedule after this
///   version number.
Query make_query(
    Version after_version);

//==============================================================================
/// Query for all Trajectories that intersect with this set of spacetime
/// regions.
///
/// \param[in] regions
///   Only query Trajectories that intersect with the specified regions.
Query make_query(
    std::vector<Region> regions);

//==============================================================================
/// Query for all Trajectories that fall within a time range.
///
/// \param[in] start_time
///   A pointer to the lower bound for the time range. Pass in a nullptr to
///   indicate that there is no lower bound.
///
/// \param[in] finish_time
///   A pointer to the upper bound for the time range. Pass in a nullptr to
///   indicate that there is no upper bound.
Query make_query(
    std::vector<std::string> maps,
    const Time* start_time,
    const Time* finish_time);

//==============================================================================
/// Query for all Trajectories that were introduced after a specified version of
/// the schedule, and which intersect with this set of spacetime regions.
///
/// \param[in] after_version
///   Only query Trajectories that were added to the schedule after this
///   version number.
///
/// \param[in] regions
///   Only query Trajectories that intersect with the specified regions.
Query make_query(
    Version after_version,
    std::vector<Region> regions);

} // namespace schedule

namespace detail {

extern template class bidirectional_iterator<
    Region,
    schedule::Query::Spacetime::Regions::IterImpl,
    schedule::Query::Spacetime::Regions
>;

extern template class bidirectional_iterator<
    const Region,
    schedule::Query::Spacetime::Regions::IterImpl,
    schedule::Query::Spacetime::Regions
>;

} // namespace detail
} // namespace rmf_traffic

#endif // RMF_TRAFFIC__SCHEDULE__QUERY_HPP
