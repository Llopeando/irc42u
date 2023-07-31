/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:16 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/31 12:40:30 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef README_H
#define README_H

#define INFO "\n " \
        "   Welcome to A O I R C server.                                                                                                                   \n" \
        "    These are the COMMANDS you may find in a IRC server as described in RFC2119 protocol, marked with a * ONLY those that A O I R C server allows.\n" \
        "                                                                                                                                                  \n" \
        "    Connection Messages                                                                                                                           \n" \
        "        * PASS         <password>                                                                                                                 \n" \
        "        * NICK         <nickname>                                                                                                                 \n" \
        "        * USER         <username> 0 * <realname>                                                                                                  \n" \
        "        * PING         <token>                                                                                                                    \n" \
        "        * OPER         <name> <password>                                                                                                          \n" \
        "        * QUIT         [<reason>]                                                                                                                 \n" \
        "                                                                                                                                                  \n" \
        "    Channel Operations                                                                                                                            \n" \
        "        * JOIN         <channel>{,<channel>}                                                                                                      \n" \
        "        * PART         <channel>{,<channel>} [<reason>]                                                                                           \n" \
        "        * TOPIC        <channel> [<topic>]                                                                                                        \n" \
        "        * NAMES        <channel>{,<channel>}                                                                                                      \n" \
        "        * LIST         [<channel>{,<channel>}]                                                                                                    \n" \
        "        * INVITE       <nickname> <channel>                                                                                                       \n" \
        "        * KICK         <channel> <user> *( "," <user> ) [<comment>]                                                                               \n" \
        "                                                                                                                                                  \n" \
        "    Server Queries and Commands                                                                                                                   \n" \
        "        * MOTD                                                                                                                                    \n" \
        "        * VERSION                                                                                                                                 \n" \
        "        * LUSERS                                                                                                                                  \n" \
        "        * MODE                                                                                                                                    \n" \
        "            User mode       //our server has only MODE +o/-o command available, to change user mode for operators                                 \n" \
        "                                                                                                                                                  \n" \
        "    Sending Messages                                                                                                                              \n" \
        "        * PRIVMSG                                                                                                                                 \n" \
        "        * NOTICE                                                                                                                                  \n" \
        "                                                                                                                                                  \n" \
        "    User-Based Queries                                                                                                                            \n" \
        "        * WHOIS                                                                                                                                   \n" \
        "        * HELP                                                                                                                                    \n" \
        "                                                                                                                                                  \n" \
        "    Operator Messages                                                                                                                             \n" \
        "        * KILL                                                                                                                                    \n" \
        "                                                                                                                                                  \n" \
        "    Optional Messages                                                                                                                             \n" \
        "        * AWAY                                                                                                                                    \n" \
        "                                                                                                                                                  \n" \
        "    Commands which are not available in our A O I R C server                                                                                      \n" \
        "        CAP            // this server does not support CAPABILITY NEGOTIATION                                                                     \n" \
        "        AUTHENTICATE                                                                                                                              \n" \
        "        ADMIN       	// no mandatory to have admin                                                                                              \n" \
        "        CONNECT     	// connection to other servers is not allowed                                                                              \n" \
        "        TIME                                                                                                                                      \n" \
        "        STATS                                                                                                                                     \n" \
        "        INFO                                                                                                                                      \n" \
        "        LINKS                                                                                                                                     \n" \
        "        USERHOST                                                                                                                                  \n" \
        "        WALLOPS                                                                                                                                   \n" \
        "        WHO                                                                                                                                       \n" \
        "        WHOWAS                                                                                                                                    \n" \
        "        REHASH                                                                                                                                    \n" \
        "        RESTART                                                                                                                                   \n" \
        "        SQUIT                                                                                                                                     \n"

#endif