package uk.ac.man.cs.comp23311.gitlabaccesstest;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.CoreMatchers.endsWith;
import static org.hamcrest.CoreMatchers.is;

import org.junit.Test;

import uk.ac.man.cs.gitconfigreader.GitConfigReader;

public class GitLabAccessCheckerTest {
	
	// Edit the string values on the line below, to make them equal to your GitLab account name and e-mail
	private final String GITLAB_NAME = "Haorui Chen";
	private final String GITLAB_EMAIL = "haorui.chen@student.manchester.ac.uk";

	
	@Test
	public void testGitConfiguredUserNameIsTheNameExpectedByGitLab() throws Exception {
		GitConfigReader configReader = GitConfigReader.getGitConfigReader();
		assertThat(configReader.getUserName(), is(GITLAB_NAME));
	}
	
	@Test
	public void testGitConfiguredUserEmailIsTheEMailExpectedByGitLab() throws Exception {
		GitConfigReader configReader = GitConfigReader.getGitConfigReader();
		String userEmail = configReader.getUserEmail();
		assertThat(userEmail, is(GITLAB_EMAIL));
	}
	
	@Test
	public void testGitConfiguredUserEmailIsAUniversityOfManchesterStudentEmail() throws Exception {
		GitConfigReader configReader = GitConfigReader.getGitConfigReader();
		String userEmail = configReader.getUserEmail();
		assertThat(userEmail, endsWith("@student.manchester.ac.uk"));
	}
	
}
