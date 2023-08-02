/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:16 by ullorent          #+#    #+#             */
/*   Updated: 2023/08/01 19:50:38 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef README_H
#define README_H


#define INFO "\n " \
        "                                                       Welcome to A O I R C server. \n" \
        "These are COMMANDS you may find in a IRC server as described in RFC2119 protocol, marked with a * ONLY those that A O I R C server allows.   \n" \
        "                                         \n" \
        "Connection Messages                      \n" \
        "    * PASS     <password>                \n" \
        "    * NICK     <nickname>                \n" \
        "    * USER     <username> 0 * <realname> \n" \
        "    * PING     <token>                   \n" \
        "    * OPER     <name> <password>         \n" \
        "    * QUIT     [<reason>]                \n" \
        "                                         \n" \
        "Channel Operations                                                                      \n" \
        "    * JOIN      <channel>{,<channel>}                                                   \n" \
        "    * PART      <channel>{,<channel>} [<reason>]                                        \n" \
        "    * TOPIC     <channel> [<topic>]                                                     \n" \
        "    * NAMES     <channel>{,<channel>}                                                   \n" \
        "    * LIST      [<channel>{,<channel>}]                                                 \n" \
        "    * INVITE    <nickname> <channel>                           //only channel operators \n" \
        "    * KICK      <channel> <user>{,<user>} [<comment>]                                   \n" \
        "                                                                                        \n" \
        "Server Queries and Commands                                                             \n" \
        "    * MOTD                                                                              \n" \
        "    * VERSION                                                                           \n" \
        "    * LUSERS                                                                            \n" \
        "    * MODE                                                     //No available by server \n" \
        "                                                      \n" \
        "Sending Messages                                      \n" \
        "    * PRIVMSG   <target>{,<target>} <text to be sent> \n" \
        "    * NOTICE    <target>{,<target>} <text to be sent> \n" \
        "                                                      \n" \
        "User-Based Queries                                    \n" \
        "    * WHOIS    [<target>] <nick>                      \n" \
        "    * HELP                                            \n" \
        "                                                      \n" \
        "Operator Messages                                     \n" \
        "    * KILL    <nickname> <comment>                    \n" \
        "                                                      \n" \
        "Optional Messages                                     \n" \
        "    * AWAY     [<text>]                               \n"

#endif