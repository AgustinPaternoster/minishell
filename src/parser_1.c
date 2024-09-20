/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpancorb <jpancorb@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:13:32 by jpancorb          #+#    #+#             */
/*   Updated: 2024/09/20 19:05:38 by jpancorb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*check_local_exe(char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && (cmd[1] == '/' || cmd[1] == '.')))
	{
		if (access(cmd, X_OK) == 0 || access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	if (access(cmd, X_OK) == 0 || access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*find_exe(char **env, char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths && paths[i])
	{
		full_path = join_path_cmd(paths[i], cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

static void	fill_cmd_full_path(t_cmd *cmds, char **env)
{
	t_cmd	*curr;

	curr = cmds;
	while (curr)
	{
		if (curr->args && curr->args[0])
		{
			curr->full_path = check_local_exe(curr->args[0]);
			if (curr->full_path)
				break ;
			curr->full_path = find_exe(env, curr->args[0]);
			if (!curr->full_path)
				printf("Command not found: %s\n", curr->args[0]);
		}
		curr = curr->next;
	}
}

static void	parse_tkn(t_token *token, t_cmd *cmd)
{
	if (token->type == WORD || token->type == VAR)
		add_arg(cmd, token->value);
	else if (token->type == RD_IN || token->type == RD_OUT
		|| token->type == APPEND || token->type == HEREDOC)
	{
		if (!token->next || token->next->type != WORD)
		{
			printf("Error: A file was expected.\n");
			return ;
		}
		token = token->next;
	//	add_red(cmd, token->value, token->prev->type);
		set_file_descriptor(cmd, cmd->full_path, token->type);
		return ;
	}
}

t_cmd	*to_parse(t_token *tokens, char **env)
{
	t_cmd	*head;
	t_cmd	*curr;

	expand_tokens(tokens, env);
	between_q(&tokens);
	head = create_cmd_node();
	if (!head)
		return (NULL);
	curr = head;
	while (tokens)
	{
		if (tokens->type == PIPE)
			curr = add_pipe(curr);
		else
		{
			parse_tkn(tokens, curr);
			if (tokens->type == RD_IN || tokens->type == RD_OUT
				|| tokens->type == APPEND || tokens->type == HEREDOC)
				tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	fill_cmd_full_path(head, env);
	return (head);
}
