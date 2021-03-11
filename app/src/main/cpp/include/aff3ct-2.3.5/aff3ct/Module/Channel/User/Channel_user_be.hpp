#ifndef CHANNEL_USER_BE_HPP_
#define CHANNEL_USER_BE_HPP_

#include <string>

#include "Tools/types.h"
#include "Module/Channel/User/Channel_user.hpp"

namespace aff3ct
{
namespace module
{

/*!
 * \class Channel_user_be
 *
 * \brief Binary Erasure Channel with user given events from a file.
 *
 * \tparam R: type of the reals (floating-point representation) in the Channel.
 */
template <typename R = float>
class Channel_user_be : public Channel_user<R>
{
public:
	using E = typename tools::matching_types<R>::B; //Event type

	Channel_user_be(const int N, const std::string &filename, const int n_frames = 1);
	virtual ~Channel_user_be() = default;

	virtual void  add_noise(const R *X_N, R *Y_N, const int frame_id = -1);

protected:
	virtual void _add_noise(const R *X_N, R *Y_N, const int frame_id = -1);
};
}
}

#endif /* CHANNEL_USER_BE_HPP_ */
