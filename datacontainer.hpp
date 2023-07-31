/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   datacontainer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 18:14:32 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/28 18:14:33 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

namespace dc{

enum Type{
	eVector,
	eList,
	nDeque
}

struct Input{
	Type type;
	uint32_t size;
	Input *pnext;
}


class DataContainer{
	DataContainer(Input *input);
	~DataContainer();

	private:
		std::map<Type, void *> data;
}

}